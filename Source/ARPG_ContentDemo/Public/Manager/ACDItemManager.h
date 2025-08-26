// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ACDItemManager.generated.h"

/**
 * - GameInstanceSubsystem 기반 아이템 매니저
 * - 인벤토리와 연동해 아이템 지급/소모 처리
 * - DataTable 기반 정의(Item.csv)
 */
UCLASS()
class ARPG_CONTENTDEMO_API UACDItemManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
