// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_ContentDemo/ThirdPersonSample/ThirdPersonSampleCharacter.h"
#include "ACDCharacter.generated.h"

class UACDInteractionSensorComponent;

UCLASS()
class ARPG_CONTENTDEMO_API AACDCharacter : public AThirdPersonSampleCharacter
{
	GENERATED_BODY()

public:
	AACDCharacter();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnInteractionTargetChanged(AActor* NewTarget, const FText& PromptText);

	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void BP_UpdateInteractionPrompt(bool bShow, const FText& PromptText);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction")
	UACDInteractionSensorComponent* InteractionSensor = nullptr;
};
