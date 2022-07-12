// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "MySaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KorttipeliGameMode.generated.h"


/** GameMode class to specify pawn and playercontroller */
UCLASS(minimalapi)
class AKorttipeliGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AKorttipeliGameMode();
};



