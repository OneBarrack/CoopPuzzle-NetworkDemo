// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACDInventoryWidget.generated.h"

struct FACDInventoryItem;

/**
 * 
 */
UCLASS()
class ARPG_CONTENTDEMO_API UACDInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable, Category="Inventory")
    void UpdateInventory(const TArray<FACDInventoryItem>& Items);

protected:
    // BP에서 바인딩할 수 있는 이벤트
    UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
    void BP_OnUpdateInventory(const TArray<FACDInventoryItem>& Items);
	
	
};
