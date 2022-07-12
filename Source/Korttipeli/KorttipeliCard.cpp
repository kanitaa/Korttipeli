

#include "KorttipeliCard.h"
#include "KorttipeliGameManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"
#include "GameFramework/PlayerController.h"
#include "KorttipeliPlayerController.h"
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>


AKorttipeliCard::AKorttipeliCard()
{
	// Structure to hold one-time initialization

	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		//mesh that is used as base for each card
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
		
		{
		}
	};

	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component 
	CardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	//set mesh based on constructor planemesh
    CardMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	//set meshes scale and location
	CardMesh->SetRelativeScale3D(FVector(1.f,1.f,0.25f));
	CardMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	//and add to dummy root
	CardMesh->SetupAttachment(DummyRoot);
	//add on click event
	CardMesh->OnClicked.AddDynamic(this, &AKorttipeliCard::CardClicked);
	



	//same stuff for background of the card, place it under the top of the card so card looks whole
	BackgroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackgroundMesh0"));
	BackgroundMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BackgroundMesh->SetRelativeScale3D(FVector(1.f, 1.f, 0.25f));
	BackgroundMesh->SetRelativeLocation(FVector(0.f, 0.f, 19.0f));
	BackgroundMesh->SetupAttachment(DummyRoot);

	//set default values
	StartLocation = FVector(0, 0, 0);
	EndLocation = FVector(0, 0, 0);
	isMoveable = false;
	hasBeenMoved = false;

	//load sound cue object
	static ConstructorHelpers::FObjectFinder<USoundCue> CardGrabSoundCueObject(TEXT("SoundCue'/Game/Audio/CardGrab_Cue.CardGrab_Cue'"));
	if (CardGrabSoundCueObject.Succeeded()) {
		CardGrab_Cue = CardGrabSoundCueObject.Object;
		//init audiocomponent and add it to root component
		GrabAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("GrabAudioComponent"));
		GrabAudioComponent->SetupAttachment(RootComponent);

	}
}
void AKorttipeliCard::BeginPlay()
{
	Super::BeginPlay();
	//if audio stuff exists
	if (GrabAudioComponent && CardGrab_Cue)
	{
		//add sound cue to audio component, do it here to avoid sound bug at start
		GrabAudioComponent->SetSound(CardGrab_Cue);
	}

}
//click function for card
void AKorttipeliCard::CardClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	//play sound cue through audio component
	GrabAudioComponent->Play();

}


//if any cards are still moveable, if not game over
void AKorttipeliCard::CheckMoves()
{
	CardGameManager->CheckForAvailableMoves();
}

//set card value based on material name
void AKorttipeliCard::SetUpCard(FString materialName) {

	if (materialName.Contains("Ace")) {
		CardValue=1;
	}
	else if (materialName.Contains("Two")) {
		CardValue = 2;
	}
	else if (materialName.Contains("Three")) {
		CardValue = 3;
	}
	else if (materialName.Contains("Four")) {
		CardValue = 4;
	}
	else if (materialName.Contains("Five")) {
		CardValue = 5;
	}
	else if (materialName.Contains("Six")) {
		CardValue = 6;
	}
	else if (materialName.Contains("Seven")) {
		CardValue = 7;
	}
	else if (materialName.Contains("Eight")) {
		CardValue = 8;
	}
	else if (materialName.Contains("Nine")) {
		CardValue = 9;
	}
	else if (materialName.Contains("Ten")) {
		CardValue = 10;
	}
	else if (materialName.Contains("Jack")) {
		CardValue = 11;
	}
	else if (materialName.Contains("Queen")) {
		CardValue = 12;
	}
	else if (materialName.Contains("King")) {
		CardValue = 13;
	}
}

