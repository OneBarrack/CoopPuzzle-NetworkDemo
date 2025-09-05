// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/ACDPlayerState.h"
#include "Inventory/ACDInventoryComponent.h"

AACDPlayerState::AACDPlayerState()
{
    bReplicates = true;
    InventoryComponent = CreateDefaultSubobject<UACDInventoryComponent>(TEXT("InventoryComponent"));
}