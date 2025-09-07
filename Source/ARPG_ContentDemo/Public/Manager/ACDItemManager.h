// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "ACDItemManager.generated.h"

class AACDPlayerState;
class UDataTable;
struct FACDItemRow;
struct FACDRewardRow;

/**
 * - GameInstanceSubsystem 기반 아이템/보상 매니저 
 * - 아이템 정의 캐시 및 조회
 * - 보상 테이블 조회 및 지급 헬퍼
 */
UCLASS()
class ARPG_CONTENTDEMO_API UACDItemManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	// Item table
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
    TObjectPtr<UDataTable> ItemTable = nullptr;

    // Reward table
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Reward")
    TObjectPtr<UDataTable> RewardTable = nullptr;

    // Item 및 Reward Row 조회 */
    bool GetItemRowByID(int32 ItemId, FACDItemRow& OutItemRow) const;
    bool GetRewardRow(FName RowName, FACDRewardRow& OutRewardRow) const;

    // Reward 지급(Server only)
    bool GrantReward(FName RowName, AACDPlayerState* ReceiverPS) const;

private:
    // ItemID -> Row
    TMap<int32, const FACDItemRow*> ItemRowMap;
};
