
#pragma once


#include "KorttipeliCard.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "KorttipeliGameManager.generated.h"


//Game manager class
UCLASS(minimalapi)
class AKorttipeliGameManager : public AActor
{
	GENERATED_BODY()

	//Dummy root component 
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;


	
public:
	AKorttipeliGameManager();
	
	
	//number of cards
	UPROPERTY(BlueprintReadOnly)
	int32 Size;
	
	//is game over and is it won
	UPROPERTY(BlueprintReadOnly)
	bool bGameOver;
	UPROPERTY(BlueprintReadOnly)
	bool bGameWon;

	//total wins and losses for showing result on end screen
	UPROPERTY(BlueprintReadOnly)
	int TotalWins;
	UPROPERTY(BlueprintReadOnly)
	int TotalLosses;

	//spawn locations (clock positions)
	UPROPERTY(Category = Cards, EditAnywhere)
	AActor *SpawnLocation[13];

	//init cards you want to spawn
	UPROPERTY(Category = Cards, EditAnywhere)
	AKorttipeliCard* CardsToSpawn[52];

	//help array to suffle the cards in cardstospawn, also used to store card actors that are spawned in game
	UPROPERTY(Category = Cards, EditAnywhere)
	AKorttipeliCard* AllCards[52];

	//material used for spawned card
	UPROPERTY(Category = Cards, EditAnywhere)
	UMaterialInterface* MaterialToUse;

	//set endlocation
	void SetCardEndLocationBasedOnCardValue(int value, AKorttipeliCard* card);
	
	//check if theres any cards to be moved
	void CheckForAvailableMoves();

	//sound cues for game ends
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
	class USoundCue* GameWon_Cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
	class USoundCue* GameLost_Cue;
private:
	//audio components for sound cues
	UAudioComponent* GameWonAudioComponent;
	UAudioComponent* GameLostAudioComponent;
protected:
	virtual void BeginPlay() override;
	
public:

	//Returns DummyRoot subobject
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	
};



