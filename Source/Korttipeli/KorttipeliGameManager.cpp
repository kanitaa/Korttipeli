

#include "KorttipeliGameManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"
#include <Korttipeli/MySaveGame.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>


#define LOCTEXT_NAMESPACE "GameManager"

AKorttipeliGameManager::AKorttipeliGameManager()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Set defaults
	Size = 52;	
	SpawnLocation[13];
	CardsToSpawn[52];	
	AllCards[52];
	MaterialToUse = NULL;

	//get sound cues with constructor helper and init audio components
	static ConstructorHelpers::FObjectFinder<USoundCue> GameWonSoundCueObject(TEXT("SoundCue'/Game/Audio/WinSound_Cue.WinSound_Cue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> GameLostSoundCueObject(TEXT("SoundCue'/Game/Audio/LoseSound_Cue.LoseSound_Cue'"));
	if (GameLostSoundCueObject.Succeeded()) {
		GameLost_Cue = GameLostSoundCueObject.Object;

		GameLostAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("GameLostAudioComponent"));
		GameLostAudioComponent->SetupAttachment(RootComponent);
		
	}
	if (GameWonSoundCueObject.Succeeded()) {
		GameWon_Cue = GameWonSoundCueObject.Object;
		
		GameWonAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("GameWonAudioComponent"));
		GameWonAudioComponent->SetupAttachment(RootComponent);

	}
	
}


void AKorttipeliGameManager::BeginPlay()
{
	Super::BeginPlay();
	//get data from UMySaveGame
	if (UMySaveGame* LoadedGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot("GameSaveData", 0)))
	{
		//success, init total losses and wins with data from save game
		TotalLosses = LoadedGame->TotalLosses;
		TotalWins = LoadedGame->TotalWins;
		UE_LOG(LogTemp, Warning, TEXT("losses: %d"), LoadedGame->TotalLosses);
		UE_LOG(LogTemp, Warning, TEXT("wins: %d"), LoadedGame->TotalWins);
	}

	//set audio cues for audiocomponents
	if (GameWonAudioComponent && GameWon_Cue)
	{
		GameWonAudioComponent->SetSound(GameWon_Cue);
	}
	if (GameLostAudioComponent && GameLost_Cue)
	{
		GameLostAudioComponent->SetSound(GameLost_Cue);
	}

	//suffle deck
	for (int32 i = 0; i < 52; i++)
	{
		int index = rand() % 52;
		AllCards[i] = CardsToSpawn[i];
		CardsToSpawn[i] = CardsToSpawn[index];
		CardsToSpawn[index] = AllCards[i];
	}
	

	//number of spawn points
	const int32 NumBlocks = 13;

	//loop to spawn each card
	for (int32 BlockIndex = 0; BlockIndex < NumBlocks; BlockIndex++)
	{
		//reduce size to know when all cards have been placed
		Size--;

		//get location for placing cards
		const FVector CardLocation = SpawnLocation[BlockIndex]->GetActorLocation();

		//spawn card in set location with face down
		AKorttipeliCard* NewCard = GetWorld()->SpawnActor<AKorttipeliCard>(CardsToSpawn[Size]->GetClass(), CardLocation, FRotator(0, -90, 180));
		//set refences for cards in world
		AllCards[Size] = NewCard;
		//if its last card, turn it around and enable movement
		if (Size == 0)
		{
			NewCard->SetActorRotation(FRotator(0, 90, 0));
			NewCard->isMoveable = true;
		}
		//set set startlocation for card
		NewCard->StartLocation = CardLocation;

		
		//get correct material for each card
		UStaticMeshComponent* Component = CardsToSpawn[Size]->GetCardMesh();
		UMaterialInterface* Material = Component->GetMaterial(0);
		MaterialToUse = Material;
		//and set that material on spawned card
		NewCard->SetCardMaterial(MaterialToUse);
	
		
		//Set value for spawned card based on material name
		NewCard->SetUpCard(MaterialToUse->GetName());
		//and set end destination for card based on value
		SetCardEndLocationBasedOnCardValue(NewCard->CardValue, NewCard);
		
	
		// Tell the card about gamemanager
		if (NewCard != nullptr)
		{
			NewCard->CardGameManager = this;
		}
		
		//If all cards havent been dealt yet, start another round for each spawn location
		if (BlockIndex == 12 && Size != 0) {
			BlockIndex = -1;	
		}
	}
}

//get card value and set end location for card based on that value
void AKorttipeliGameManager::SetCardEndLocationBasedOnCardValue(int value, AKorttipeliCard* NewCard) {
	switch (value)
	{
	case 1: NewCard->EndLocation = SpawnLocation[0]->GetActorLocation();
		break;
	case 2: NewCard->EndLocation = SpawnLocation[1]->GetActorLocation();
		break;
	case 3: NewCard->EndLocation = SpawnLocation[2]->GetActorLocation();
		break;
	case 4: NewCard->EndLocation = SpawnLocation[3]->GetActorLocation();
		break;
	case 5: NewCard->EndLocation = SpawnLocation[4]->GetActorLocation();
		break;
	case 6: NewCard->EndLocation = SpawnLocation[5]->GetActorLocation();
		break;
	case 7: NewCard->EndLocation = SpawnLocation[6]->GetActorLocation();
		break;
	case 8: NewCard->EndLocation = SpawnLocation[7]->GetActorLocation();
		break;
	case 9: NewCard->EndLocation = SpawnLocation[8]->GetActorLocation();
		break;
	case 10: NewCard->EndLocation = SpawnLocation[9]->GetActorLocation();
		break;
	case 11: NewCard->EndLocation = SpawnLocation[10]->GetActorLocation();
		break;
	case 12: NewCard->EndLocation = SpawnLocation[11]->GetActorLocation();
		break;
	case 13: NewCard->EndLocation = SpawnLocation[12]->GetActorLocation();
		break;
	default: NewCard->EndLocation = FVector(0, 0, 0);
	}
}

//check if theres any possible moves left
void AKorttipeliGameManager::CheckForAvailableMoves() {
	
	int moveableCards=0;
	int correctPosition = 0;
	//go through all cards
	for (int32 i = 0; i < 52; i++)
	{
		//if not all cards have been moved and there is a card that is still moveable
		if (!AllCards[i]->hasBeenMoved && AllCards[i]->isMoveable) {
			//Movable card found
			moveableCards++;
		}
	}
	//out of moves, no moveable cards
	if (moveableCards == 0) {
		bGameOver = true;

		//check end positions for all cards
		for (int32 i = 0; i < 52; i++)
		{
			if (FVector::DistSquared(AllCards[i]->GetActorLocation(), AllCards[i]->EndLocation) < 4000) {
				correctPosition++;
			}
			//turn remaining cards around
			if (AllCards[i]->GetActorRotation() != FRotator(0, 90, 0)) {
				AllCards[i]->SetActorRotation(FRotator(0, 90, 0));
			}
		}
		//if all cards are in correct place, game is won
		if (correctPosition == 52) {
			bGameWon = true;
			GameWonAudioComponent->Play(); //play win audio cue
			//save win
			UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
			// Set data on the savegame object.
			SaveGameInstance->TotalWins= TotalWins+ 1;

			//save data
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, "GameSaveData", 0);
			TotalWins = SaveGameInstance->TotalWins;
				
			
		}
		else {
			
			GameLostAudioComponent->Play(); //play game lost audio cue
			//save lose
			UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
			// Set data on the savegame object.
			SaveGameInstance->TotalLosses = TotalLosses + 1;

			//save data
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, "GameSaveData", 0);
			TotalLosses = SaveGameInstance->TotalLosses;
				
			
		}
		
	}
	else {
		//still some moves left continue game as it is
	}
	


}
#undef LOCTEXT_NAMESPACE
