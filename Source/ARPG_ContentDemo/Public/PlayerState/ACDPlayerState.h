// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ACDPlayerState.generated.h"

class UACDInventoryComponent;

/**
 * (구현 중)
 * 인벤토리 / 퀘스트 진행 반영
 */
UCLASS()
class ARPG_CONTENTDEMO_API AACDPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
    AACDPlayerState();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
    TObjectPtr<UACDInventoryComponent> InventoryComponent;
};
