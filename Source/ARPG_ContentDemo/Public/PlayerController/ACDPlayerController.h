// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ACDPlayerController.generated.h"

/**
 * 플레이어 컨트롤러
 * - 인터렉션 UI 처리 전담
 * - 포제션 변경 시 Sensor 델리게이트를 안전하게 바인딩/해제
 * - OnInteractionTargetChanged: 인터렉션 타겟 변경 대응
 */
UCLASS()
class ARPG_CONTENTDEMO_API AACDPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;

public:
    UFUNCTION(BlueprintNativeEvent)
    void OnInteractionTargetChanged(AActor* NewTarget, const FText& PromptText);
    virtual void OnInteractionTargetChanged_Implementation(AActor* NewTarget, const FText& PromptText);

private:
    UFUNCTION()
    void BindToPawnSensor(APawn* InPawn);
};
