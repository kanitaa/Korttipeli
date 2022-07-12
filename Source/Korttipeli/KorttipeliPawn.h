

#pragma once
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "KorttipeliPawn.generated.h"

UCLASS(config=Game)
class AKorttipeliPawn : public APawn
{
	GENERATED_UCLASS_BODY()

public:

	virtual void Tick(float DeltaSeconds) override;
	//is it time to enable another card and turn it around on board
	bool enableCard;
	//are we dragging a card?
	bool draggingCard;

	//sound for flipping card
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
	class USoundCue* CardFlip_Cue;
	//sound for starting game
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
	class USoundCue* GameStart_Cue;
private:
	//audio components for each sound cue
	UAudioComponent* FlipAudioComponent;
	UAudioComponent* GameStartAudioComponent;

protected:
	//trace cast from mouse to see if its on card
	void TraceForCard(const FVector& Start, const FVector& End, bool bDrawDebugHelpers);
	//traced card
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class AKorttipeliCard* CurrentCardFocus;
	//card to be moved
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class AKorttipeliCard* CardToBeMoved;
	virtual void BeginPlay() override;
};
