// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ACDPlayerController.generated.h"

/**
 * 
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
