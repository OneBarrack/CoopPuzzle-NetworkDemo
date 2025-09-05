// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACDEnum.generated.h"

// Reward
UENUM(BlueprintType)
enum class ERewardType : uint8
{
    None = 0,
    Quest,
    Chest,
    ERewardType_Max UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(ERewardType, ERewardType::ERewardType_Max)