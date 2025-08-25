// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ACDQuestManager.generated.h"

/**
 * - GameInstanceSubsystem 기반 퀘스트 매니저
 * - 퀘스트 수락/진행/완료 책임 및 중복 보상 방지
 */
UCLASS()
class ARPG_CONTETDEMO_API UACDQuestManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
