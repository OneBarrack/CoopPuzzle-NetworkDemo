// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/ACDDataRegistry.h"

void UACDDataRegistry::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("[%s] DataRegistry Initialized"), ANSI_TO_TCHAR(__FUNCTION__));
}
