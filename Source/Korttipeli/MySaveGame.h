// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class KORTTIPELI_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()


public:

	//save slot

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	//values to be saved

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int TotalWins;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	int TotalLosses;


	
	UMySaveGame();
	
};
