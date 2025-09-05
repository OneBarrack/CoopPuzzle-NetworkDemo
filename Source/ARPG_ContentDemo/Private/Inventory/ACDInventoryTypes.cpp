// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/ACDInventoryTypes.h"

int32 FACDInventoryList::FindIndex(int32 ItemID) const
{
	for (int32 Index = 0; Index < Items.Num(); ++Index)
    {
        if (Items[Index].ItemID == ItemID)
        {
            return Index;
        }
    }

	return INDEX_NONE;
}

bool FACDInventoryList::AddItem(int32 ItemID, int32 ItemCount)
{
    if (ItemID <= 0 || ItemCount <= 0) 
        return false;

    if (const int32 Index = FindIndex(ItemID); Index != INDEX_NONE)
    {
        Items[Index].Quantity += ItemCount;
        MarkItemDirty(Items[Index]);
    }
    else
    {
        FACDInventoryItem& Item = Items.AddDefaulted_GetRef();
        Item.ItemID = ItemID;
        Item.Quantity = ItemCount;
        MarkItemDirty(Item);
    }

    return true;
}

bool FACDInventoryList::RemoveItem(int32 ItemID, int32 ItemCount)
{
    if (ItemID <= 0 || ItemCount <= 0) 
        return false;

    const int32 Index = FindIndex(ItemID);
    if (Index == INDEX_NONE)
        return false;

    FACDInventoryItem& Item = Items[Index];
    if (ItemCount < Item.Quantity)
    { 
        Item.Quantity -= ItemCount; 
        MarkItemDirty(Item);
    }
    else 
    { 
        MarkItemDirty(Item); 
        Items.RemoveAt(Index);
    }

    return true;
}