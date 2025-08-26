// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_ContentDemo/ThirdPersonSample/ARPG_ContentDemoCharacter.h"
#include "ACDCharacter.generated.h"

class UACDInteractionSensorComponent;

UCLASS()
class ARPG_CONTENTDEMO_API AACDCharacter : public AARPG_ContentDemoCharacter
{
	GENERATED_BODY()

public:
	AACDCharacter();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnTargetChanged(AActor* NewTarget, const FText& PromptText);

	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void BP_UpdateInteractPrompt(bool bShow, const FText& PromptText);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction")
	UACDInteractionSensorComponent* InteractionSensor = nullptr;
};
