// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/ACDQuestManager.h"

void UACDQuestManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("[%s] QuestManager Initialized"), ANSI_TO_TCHAR(__FUNCTION__));
}

void UACDQuestManager::Deinitialize()
{
	Super::Deinitialize();
}
