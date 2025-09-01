// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACDCharacterBase.h"
#include "ACDCharacter.generated.h"

class UACDInteractionSensorComponent;

/**
 * - InteractionSensor 컴포넌트를 보유
 * - Interact 입력 처리 (서버 RPC)
 */
UCLASS()
class ARPG_CONTENTDEMO_API AACDCharacter : public AACDCharacterBase
{
	GENERATED_BODY()

public:
	AACDCharacter();

	UFUNCTION(BlueprintCallable, Category="Interact")
	void Interact();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<UACDInteractionSensorComponent> InteractionSensor = nullptr;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Input", meta=(AllowPrivateAccess="true"))
    TObjectPtr<UInputAction> InteractAction;
};
