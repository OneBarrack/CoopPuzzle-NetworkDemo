// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/ACDPlayerState.h"
#include "Inventory/ACDInventoryComponent.h"
#include "Net/UnrealNetwork.h"

AACDPlayerState::AACDPlayerState()
{
    bReplicates = true;
    InventoryComponent = CreateDefaultSubobject<UACDInventoryComponent>(TEXT("InventoryComponent"));
}

void AACDPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AACDPlayerState, InventoryComponent);
}
