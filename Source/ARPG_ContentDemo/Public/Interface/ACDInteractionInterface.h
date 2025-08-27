// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ACDInteractionInterface.generated.h"

/**
 * 액터가 상호작용 대상임을 나타내는 계약 인터페이스
 */
UINTERFACE(Blueprintable)
class ARPG_CONTENTDEMO_API UACDInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class ARPG_CONTENTDEMO_API IACDInteractionInterface
{
	GENERATED_BODY()

public:
    // 상호작용 가능 여부
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interact")
    bool CanInteract(AActor* InstigatorActor) const;

    // 상호작용 실행
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interact")
    void DoInteract(AActor* InstigatorActor);
};
