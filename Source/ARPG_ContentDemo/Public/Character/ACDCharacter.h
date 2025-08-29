// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_ContentDemo/ThirdPersonSample/ThirdPersonSampleCharacter.h"
#include "ACDCharacter.generated.h"

class UACDInteractionSensorComponent;

/**
 * 캐릭터 클래스
 * - InteractionSensor 컴포넌트를 보유
 * - 입력 처리만 담당, 로직 대부분은 Sensor/Controller에 위임
 */
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
	TObjectPtr<UACDInteractionSensorComponent> InteractionSensor = nullptr;
};
