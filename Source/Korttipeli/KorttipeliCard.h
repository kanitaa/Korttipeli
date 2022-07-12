
#pragma once
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KorttipeliCard.generated.h"

/** a card that can be clicked */
UCLASS(minimalapi)
class AKorttipeliCard : public AActor
{
	GENERATED_BODY()

	// Dummy root component 
	UPROPERTY(Category = Card, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	//StaticMesh component for the clickable card ("face up" mesh")
	UPROPERTY(Category = Card, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CardMesh;

	//StaticMesh component for background of card
	UPROPERTY(Category = Card, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BackgroundMesh;

public:
	AKorttipeliCard();

	
	// value of card 
	UPROPERTY(Category = CardValue, EditAnywhere)
	int CardValue=0;

	//is card moveable
	bool isMoveable;
	//if card has already been moved then dont make it moveable again
	bool hasBeenMoved;

	// start and end locations for card
	UPROPERTY(Category = CardValue, EditAnywhere)
	FVector StartLocation;
	UPROPERTY(Category = CardValue, EditAnywhere)
	FVector EndLocation;

	//sound for clicking card
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
	class USoundCue* CardGrab_Cue;
	


	//game manager
	UPROPERTY()
	class AKorttipeliGameManager* CardGameManager;

	// card clicked function
	UFUNCTION()
	void CardClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	//check if theres any moveable cards in game
	void CheckMoves();

	//set up card value based on material name, card value is used to get correct end location
	UFUNCTION()
	void SetUpCard(FString materialName);


public:
	// Returns DummyRoot subobject 
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	// Returns BlockMesh subobject 
	FORCEINLINE class UStaticMeshComponent* GetCardMesh() const { return CardMesh; }
	//set card mesh material
	FORCEINLINE class UMaterialInstance* SetCardMaterial(UMaterialInterface* Material) { CardMesh->SetMaterial(0, Material); return 0; }

	// Returns BlockMesh subobject 
	FORCEINLINE class UStaticMeshComponent* GetBackgroundMesh() const { return BackgroundMesh; }
protected:
	virtual void BeginPlay() override;

private:
	//audio component for click sound
	UAudioComponent* GrabAudioComponent;

};



