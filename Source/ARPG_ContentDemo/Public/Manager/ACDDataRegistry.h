// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ACDDataRegistry.generated.h"

/**
 * (추후 확장 예정) 
 * - GameInstanceSubsystem 기반 데이터 접근 통로
 * - csv 파일을 통해 UDataTable 로드 및 캐싱
 */
UCLASS()
class ARPG_CONTENTDEMO_API UACDDataRegistry : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    const UDataTable* GetQuestTable() const { return QuestTable; }
    const UDataTable* GetItemTable()  const { return ItemTable; }

private:
    UPROPERTY() 
    TObjectPtr<UDataTable> QuestTable = nullptr;

    UPROPERTY() 
    TObjectPtr<UDataTable> ItemTable  = nullptr;
};
