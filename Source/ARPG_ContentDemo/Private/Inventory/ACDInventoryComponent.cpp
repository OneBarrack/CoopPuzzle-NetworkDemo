// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ACDInventoryComponent.h"
#include "Net/UnrealNetwork.h"

UACDInventoryComponent::UACDInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UACDInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION(UACDInventoryComponent, Inventory, COND_OwnerOnly);
}

int32 UACDInventoryComponent::GetQuantity(int32 ItemID) const
{
    for (const FACDInventoryItem& Item : Inventory.GetItems())
    {
        if (Item.ItemID == ItemID)
        {
            return Item.Quantity;
        }
    }

    return 0;
}

void UACDInventoryComponent::AddItem(int32 ItemID, int32 Count)
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
        return;

    if (Inventory.AddItem(ItemID, Count))
    {
        BroadcastUpdatedInventory();
    }
}

void UACDInventoryComponent::ConsumeItem(int32 ItemID, int32 Count)
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
        return;

    if (Inventory.RemoveItem(ItemID, Count))
    {
        BroadcastUpdatedInventory();
    }
}

void UACDInventoryComponent::Server_AddItem_Implementation(int32 ItemID, int32 Count)
{
    AddItem(ItemID, Count);
}

void UACDInventoryComponent::Server_ConsumeItem_Implementation(int32 ItemID, int32 Count)
{
    ConsumeItem(ItemID, Count);
}

void UACDInventoryComponent::BroadcastUpdatedInventory()
{
    OnInventoryUpdated.Broadcast(Inventory.GetItems());
}

void UACDInventoryComponent::OnRep_Inventory()
{
    BroadcastUpdatedInventory();
}
