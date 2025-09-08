// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ACDInventoryWidget.h"

void UACDInventoryWidget::UpdateInventory(const TArray<FACDInventoryItem>& Items)
{
    BP_OnUpdateInventory(Items);
}



