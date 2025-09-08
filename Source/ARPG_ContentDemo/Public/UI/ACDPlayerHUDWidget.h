// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ACDPlayerHUDWidget.generated.h"

struct FACDInventoryItem;

/** 메인 오버레이 HUD 위젯 (연출/레이아웃은 BP에서 구현) */
UCLASS()
class ARPG_CONTENTDEMO_API UACDPlayerHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // 토스트 메시지 표시
    UFUNCTION(BlueprintCallable, Category="HUD|Toast")
    void ShowToastMessage(const FText& Message, float DisplaySeconds = 1.2f);

    // 퀘스트 진행도 갱신 (예: "레버 1/2")
    UFUNCTION(BlueprintCallable, Category="HUD|Quest")
    void UpdateQuestProgress(const FText& Title, int32 CurrentCount, int32 TargetCount);    

    // 상호작용 프롬프트 표시/숨김
    UFUNCTION(BlueprintCallable, Category="HUD|Interaction")
    void SetInteractionPrompt(const FText& PromptText, bool bIsVisible);    

    // 인벤토리UI 업데이트
    UFUNCTION(BlueprintCallable, Category="HUD|Inventory")
    void OpenInventory();

    UFUNCTION(BlueprintCallable, Category="HUD|Inventory")
    void UpdateInventoryUI(const TArray<FACDInventoryItem>& Items);

    // BP가 구현하는 이벤트 (C++은 시그니처만 보장)
    UFUNCTION(BlueprintImplementableEvent, Category="HUD|Toast")
    void BP_OnShowToastMessage(const FText& Message, float DisplaySeconds);

    UFUNCTION(BlueprintImplementableEvent, Category="HUD|Quest")
    void BP_OnUpdateQuestProgress(const FText& Title, int32 CurrentCount, int32 TargetCount);

    UFUNCTION(BlueprintImplementableEvent, Category="HUD|Interaction")
    void BP_OnSetInteractionPrompt(const FText& PromptText, bool bIsVisible);

    UFUNCTION(BlueprintImplementableEvent, Category="HUD|Inventory")
    void BP_OpenInventory();

    UFUNCTION(BlueprintImplementableEvent, Category="HUD|Inventory")
    void BP_OnUpdateInventoryUI(const TArray<FACDInventoryItem>& Items);
};
