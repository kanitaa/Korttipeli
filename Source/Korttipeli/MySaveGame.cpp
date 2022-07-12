// Fill out your copyright notice in the Description page of Project Settings.
#include "MySaveGame.h"
#include "Korttipeli.h"



UMySaveGame::UMySaveGame() {
	SaveSlotName = TEXT("GameSaveData");
	TotalWins = 0;
	TotalLosses = 0;
}

