// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "ACDInventoryTypes.generated.h"

USTRUCT(BlueprintType)
struct FACDInventoryItem : public FFastArraySerializerItem {
    GENERATED_BODY()

public:
    bool IsEmpty() const { return ItemID <= 0 || Quantity <= 0; }

public:
    UPROPERTY(BlueprintReadOnly) 
    int32 ItemID = 0; // 서버/DB/네트워크 핵심 키

    UPROPERTY(BlueprintReadOnly) 
    int32 Quantity = 0;
};

USTRUCT()
struct FACDInventoryList : public FFastArraySerializer {
    GENERATED_BODY()

public:
    bool NetDeltaSerialize(FNetDeltaSerializeInfo& P) {
        return FFastArraySerializer::FastArrayDeltaSerialize<FACDInventoryItem, FACDInventoryList>(Items, P, *this);
    }

    int32 FindIndex(int32 ItemID) const;
    bool AddItem(int32 ItemID, int32 ItemCount);
    bool RemoveItem(int32 ItemID, int32 ItemCount);
    const TArray<FACDInventoryItem>& GetItems() const { return Items; };

private:
    UPROPERTY() 
    TArray<FACDInventoryItem> Items;
};

template<> struct TStructOpsTypeTraits<FACDInventoryList> : TStructOpsTypeTraitsBase2<FACDInventoryList> {
    enum { WithNetDeltaSerializer = true };
};
