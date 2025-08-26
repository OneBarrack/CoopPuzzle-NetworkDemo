// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/ACDInteractionManager.h"

void UACDInteractionManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("[%s] InteractionManager Initialized"), ANSI_TO_TCHAR(__FUNCTION__));
}

void UACDInteractionManager::Deinitialize()
{
	Super::Deinitialize();
}
