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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction")
	UACDInteractionSensorComponent* InteractionSensor = nullptr;
};
