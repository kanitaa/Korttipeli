// Copyright Epic Games, Inc. All Rights Reserved.

#include "KorttipeliGameMode.h"
#include "KorttipeliPlayerController.h"
#include "KorttipeliPawn.h"

AKorttipeliGameMode::AKorttipeliGameMode()
{
	// no pawn by default
	DefaultPawnClass = AKorttipeliPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AKorttipeliPlayerController::StaticClass();
}
