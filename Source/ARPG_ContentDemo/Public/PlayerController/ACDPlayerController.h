// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "ACDPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * 플레이어 컨트롤러
 * - 인터렉션 UI 처리 전담
 * - 포제션 변경 시 Sensor 델리게이트를 안전하게 바인딩/해제
 * - OnInteractionTargetChanged: 인터렉션 타겟 변경 대응
 * - EnhancedInput MappingContext 관리
 * - UI 모드 전환 처리
 */
UCLASS()
class ARPG_CONTENTDEMO_API AACDPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;

public:
    UFUNCTION(BlueprintNativeEvent)
    void OnInteractionTargetChanged(AActor* NewTarget);
    virtual void OnInteractionTargetChanged_Implementation(AActor* NewTarget);

private:
    UFUNCTION()
    void BindToPawnSensor(APawn* NewPawn);

    UEnhancedInputLocalPlayerSubsystem* GetInputSubsystem() const;
    void AddContext(UInputMappingContext* Context, int32 Priority) const;
    void RemoveContext(UInputMappingContext* Context) const;

    void ToggleUIMode();
    bool bUIMode = false;

public:
    // 게임플레이 컨텍스트(이동/상호작용)
    UPROPERTY(EditDefaultsOnly, Category=Input)
    TObjectPtr<UInputMappingContext> GameplayContext;

    // UI 컨텍스트(인벤토리/메뉴)
    UPROPERTY(EditDefaultsOnly, Category=Input)
    TObjectPtr<UInputMappingContext> UIContext;

    // UI 액션
    UPROPERTY(EditDefaultsOnly, Category="Input")
    TObjectPtr<UInputAction> ToggleUIAction;
};
