// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/ACDEnum.h"
#include "ACDStruct.generated.h"

// 아이템 정의 Row
USTRUCT(BlueprintType)
struct FACDItemRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 ItemID = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UTexture2D* Icon = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly) 
    int32 MaxStack = 99;
};

// 보상 Row
USTRUCT(BlueprintType)
struct FACDRewardRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly) 
    ERewardType Type = ERewardType::None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly) 
    TArray<FACDRewardItem> Items;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin="0")) 
    int32 Exp   = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin="0")) 
    int32 Money = 0;
};
