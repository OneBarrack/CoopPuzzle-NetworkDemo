// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/ACDItemManager.h"

void UACDItemManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("[%s] ItemManager Initialized"), ANSI_TO_TCHAR(__FUNCTION__));
}

void UACDItemManager::Deinitialize()
{
	Super::Deinitialize();
}