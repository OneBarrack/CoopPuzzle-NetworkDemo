// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/ACDItemManager.h"
#include "Settings/ACDGameDataSettings.h"
#include "Inventory/ACDInventoryComponent.h"
#include "Common/ACDStruct.h"
#include "PlayerState/ACDPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogACDItemManager, Log, All);

void UACDItemManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // GameDataSettings에서 소프트 레퍼런스 로드
    const UACDGameDataSettings* Settings = GetDefault<UACDGameDataSettings>();
    if (!Settings)
    {
        UE_LOG(LogACDItemManager, Error, TEXT("[%s] GameDataSettings not found."), ANSI_TO_TCHAR(__FUNCTION__));
        return;
    }

    if (!Settings->ItemTable.IsNull())   ItemTable = Settings->ItemTable.LoadSynchronous();
    if (!Settings->RewardTable.IsNull()) RewardTable = Settings->RewardTable.LoadSynchronous();

    // Item cache
    ItemRowMap.Reset();

    if (ItemTable)
    {
        const TArray<FName> RowNames = ItemTable->GetRowNames();
        ItemRowMap.Reserve(RowNames.Num()); // 미리 캐시 사이즈 확보

        int32 Valid = 0;
        int32 Skipped = 0;
        static const FString ItemContextString(TEXT("ItemCache"));
        for (const FName& RowName : RowNames)
        {
            if (const FACDItemRow* Row = ItemTable->FindRow<FACDItemRow>(RowName, ItemContextString))
            {
                if (Row->ItemID > 0 && !ItemRowMap.Contains(Row->ItemID))
                {
                    ItemRowMap.Add(Row->ItemID, Row);
                    ++Valid;
                }
                else
                {
                    ++Skipped;
                }
            }
        }

        UE_LOG(LogACDItemManager, Log, TEXT("[%s] Item cache built. Valid(%d) Skipped(%d)"), ANSI_TO_TCHAR(__FUNCTION__), Valid, Skipped);
    }
    else
    {
        UE_LOG(LogACDItemManager, Warning, TEXT("[%s] ItemTable not set or failed to load."), ANSI_TO_TCHAR(__FUNCTION__));
    }

    // Reward
    if (RewardTable)
    {
        UE_LOG(LogACDItemManager, Log, TEXT("[%s] RewardTable ready. Rows(%d)"), ANSI_TO_TCHAR(__FUNCTION__), RewardTable->GetRowNames().Num());
    }
    else
    {
        UE_LOG(LogACDItemManager, Warning, TEXT("[%s] RewardTable not set or failed to load."), ANSI_TO_TCHAR(__FUNCTION__));
    }

    UE_LOG(LogACDItemManager, Log, TEXT("[%s] ItemManager Initialized"), ANSI_TO_TCHAR(__FUNCTION__));
}

bool UACDItemManager::GetItemRowByID(int32 ItemId, FACDItemRow& OutItemRow) const
{
    if (const FACDItemRow* const* FoundRowPtr = ItemRowMap.Find(ItemId))
    {
        OutItemRow = **FoundRowPtr;
        return true;
    }

    return false;
}

bool UACDItemManager::GetRewardRow(FName RowName, FACDRewardRow& OutRewardRow) const
{
    if (!RewardTable || RowName.IsNone()) 
        return false;

    static const FString RewardContextString(TEXT("RewardLookup"));
    if (const FACDRewardRow* Row = RewardTable->FindRow<FACDRewardRow>(RowName, RewardContextString))
    {
        OutRewardRow = *Row; 
        return true;
    }

    return false;
}

bool UACDItemManager::GrantReward(FName RowName, AACDPlayerState* ReceiverPS) const
{
    if (!ReceiverPS || !ReceiverPS->HasAuthority()) 
        return false;

    FACDRewardRow RewardRow;
    if (!GetRewardRow(RowName, RewardRow)) 
        return false;

    int32 GrantedItemCount = 0;
    if (UACDInventoryComponent* InventoryComponent = ReceiverPS->FindComponentByClass<UACDInventoryComponent>())
    {        
        for (const FACDRewardItem& RewardItem : RewardRow.Items)
        {
            if (RewardItem.ItemID > 0 && RewardItem.Count > 0)
            {
                InventoryComponent->AddItem(RewardItem.ItemID, RewardItem.Count); 
                ++GrantedItemCount;
            }
        }

        UE_LOG(LogACDItemManager, Log, TEXT("[%s] Granted reward '%s' items(%d)"), ANSI_TO_TCHAR(__FUNCTION__) , *RowName.ToString(), GrantedItemCount);
    }

    // Exp, Money -> 추후
    const bool bGrantedNonItem = (RewardRow.Exp > 0 || RewardRow.Money > 0);
    return (GrantedItemCount > 0) || bGrantedNonItem;
}
