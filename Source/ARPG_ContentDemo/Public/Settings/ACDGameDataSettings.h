// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ACDGameDataSettings.generated.h"

/**
 * 게임 데이터 세팅
 * 프로젝트 설정에 전용 패널을 만들어, 디자이너가 DataTable 레퍼런스 지정할 수 있도록
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "ARPG Content Demo - Data"))
class ARPG_CONTENTDEMO_API UACDGameDataSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, Config, Category="Tables")
    TSoftObjectPtr<UDataTable> ItemTable;

    UPROPERTY(EditAnywhere, Config, Category="Tables")
    TSoftObjectPtr<UDataTable> RewardTable;
};
