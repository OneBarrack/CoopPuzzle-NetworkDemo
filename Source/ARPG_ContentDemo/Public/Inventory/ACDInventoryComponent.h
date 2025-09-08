// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/ACDInventoryTypes.h"
#include "ACDInventoryComponent.generated.h"

/** 
* 플레이어별 인벤토리
*/
UCLASS(ClassGroup=(ACD), meta=(BlueprintSpawnableComponent))
class ARPG_CONTENTDEMO_API UACDInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UACDInventoryComponent();

    UFUNCTION(BlueprintPure)
    int32 GetQuantity(int32 ItemID) const;

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly) 
    void AddItem(int32 ItemID, int32 Count);

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
    void ConsumeItem(int32 ItemID, int32 Count);

    UFUNCTION(Server, Reliable) 
    void Server_AddItem(int32 ItemID, int32 Count);
    
    UFUNCTION(Server, Reliable) 
    void Server_ConsumeItem(int32 ItemID, int32 Count);

    const FACDInventoryList& GetInventory() const { return Inventory; };

protected:
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    UFUNCTION()
    void BroadcastUpdatedInventory();
    
    UFUNCTION()
    void OnRep_Inventory();

public:
    UPROPERTY(ReplicatedUsing=OnRep_Inventory)
    FACDInventoryList Inventory;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdated, const TArray<FACDInventoryItem>&, Items);
    UPROPERTY(BlueprintAssignable) 
    FOnInventoryUpdated OnInventoryUpdated;
};

