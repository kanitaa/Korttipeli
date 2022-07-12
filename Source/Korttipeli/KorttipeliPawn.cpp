

#include "KorttipeliPawn.h"
#include "KorttipeliCard.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>




AKorttipeliPawn::AKorttipeliPawn(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	CardToBeMoved = NULL;
	//pawn posses playercontroller
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	//load sound cue objects and init audio components
	static ConstructorHelpers::FObjectFinder<USoundCue> CardFlipSoundCueObject(TEXT("SoundCue'/Game/Audio/CardFlip_Cue.CardFlip_Cue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> GameStartSoundCueObject(TEXT("SoundCue'/Game/Audio/GameStart_Cue.GameStart_Cue'"));
	if (CardFlipSoundCueObject.Succeeded()) {
		CardFlip_Cue = CardFlipSoundCueObject.Object;
		GameStart_Cue = GameStartSoundCueObject.Object;
		FlipAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FlipAudioComponent"));
		FlipAudioComponent->SetupAttachment(RootComponent);

		GameStartAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("GameStartAudioComponent"));
		GameStartAudioComponent->SetupAttachment(RootComponent);
		//set game start cue here so it will play once when game starts
		GameStartAudioComponent->SetSound(GameStart_Cue);
	}
}
void AKorttipeliPawn::BeginPlay()
{
	Super::BeginPlay();
	if (FlipAudioComponent && CardFlip_Cue)
	{
		//set flip card sound cue here so it wont play once when game starts
		FlipAudioComponent->SetSound(CardFlip_Cue);
	}

}
void AKorttipeliPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//if theres playercontroller
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
			//get start, end and direction for trace
			FVector Start, Dir, End;
			PC->DeprojectMousePositionToWorld(Start, Dir);
			End = Start + (Dir * 8000.0f);

			
			//look for card under mouse
			TraceForCard(Start, End, false);
			
			

			//there is card to be moved
			if (CardToBeMoved != NULL) {
				//set card world location from mouse position for dragging the card
				FVector WorldLocation;
				FVector WorldDirection;
				float DistanceAboveGround = -1400.0f; //z location of dragged card

				PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
				//calculate actor location to match mouse position
				FVector PlaneOrigin(0.0f, 0.0f, DistanceAboveGround);
				FVector ActorWorldLocation = FMath::LinePlaneIntersection(
					WorldLocation,
					WorldLocation + WorldDirection,
					PlaneOrigin,
					FVector::UpVector);

				//if mousebutton is clicked and mouse is on the card
				if(PC->WasInputKeyJustPressed(EKeys::LeftMouseButton) && FVector::DistSquared(CardToBeMoved->GetActorLocation(), ActorWorldLocation) < 6500)
				{
					draggingCard = true; //we're dragging the card
				}
				//if mousebutton is hold down and dragging is true (mouse was clicked on card)
				if (PC->IsInputKeyDown(EKeys::LeftMouseButton) && draggingCard) {

					CardToBeMoved->SetActorLocation(ActorWorldLocation); //use mouse location for card
						
					}
				//released left mouse button
				if (PC->WasInputKeyJustReleased(EKeys::LeftMouseButton)) {
					//if dragged card is close to correct location
					if (FVector::DistSquared(CardToBeMoved->GetActorLocation(), CardToBeMoved->EndLocation)<3500)
					{
						//Correct location, set position to end location and put it "to bottom of the card pile"
						CardToBeMoved->SetActorLocation(FVector(CardToBeMoved->EndLocation.X, CardToBeMoved->EndLocation.Y, -1410));
						CardToBeMoved->isMoveable = false; //not moveable card anymore
						CardToBeMoved->hasBeenMoved = true; //this card has been moved to correct position
					
						//enable next card
						enableCard = true;
						
				
					}
					else {
						//Wrong location, moving card back to start location
						CardToBeMoved->SetActorLocation(CardToBeMoved->StartLocation);
						
					}
					draggingCard = false; //not dragging card anymore
				}
			
			}
	}
	
}



//line trace from mouse
void AKorttipeliPawn::TraceForCard(const FVector& Start, const FVector& End, bool bDrawDebugHelpers)
{
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	
	//if we hit something
	if (HitResult.Actor.IsValid())
	{
		//if there is AKorttipeliCard Actor under hit
		AKorttipeliCard* HitCard = Cast<AKorttipeliCard>(HitResult.Actor.Get());
		//no current focus yet and we're not dragging old card
		if (CurrentCardFocus != HitCard && !draggingCard)
		{
			//focus what we hit with linetrace
			CurrentCardFocus = HitCard;
			//if next card movement needs to be enabled
			if (enableCard) {
				//set card to be moved to null so another one can be set
				CardToBeMoved = NULL;
				CurrentCardFocus->isMoveable = true; //set moveable value to true so card can be moved
				CurrentCardFocus->SetActorRotation(FRotator(0, 90, 0)); //flip card around to face it up
				FlipAudioComponent->Play();
				CurrentCardFocus->CheckMoves(); //check if there are any valid cards for movement
				enableCard = false;
			}
			
		}
	
	}
	//if no card to be moved is set yet and there is card under mouse trace
	if (CardToBeMoved == NULL && CurrentCardFocus!=NULL) {
		//if that mouse trace card is moveable and not moved yet
		if (CurrentCardFocus->isMoveable && !CurrentCardFocus->hasBeenMoved) {
			CardToBeMoved = CurrentCardFocus;
			
		}
	}
	
}