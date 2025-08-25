// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ACDInteractionManager.generated.h"

/**
 * - WorldSubsystem 기반 상호작용 허브
 * - 레버/문/상자 등 Interactable 오브젝트들과 상호작용
 * - 이벤트 기반 트리거
 * - Net-Ready: 멀티 전환 시 RPC/Replication 삽입 용이
 */
UCLASS()
class ARPG_CONTETDEMO_API UACDInteractionManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
