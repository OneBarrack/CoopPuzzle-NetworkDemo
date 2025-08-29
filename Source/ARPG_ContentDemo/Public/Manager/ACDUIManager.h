// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "ACDUIManager.generated.h"

class UACDPlayerHUDWidget;

/** LocalPlayer 단위 UI 매니저: HUD 1회 생성/재사용 + 간단 API 제공 */
UCLASS()
class ARPG_CONTENTDEMO_API UACDUIManager : public ULocalPlayerSubsystem
{
    GENERATED_BODY()

public:
    UACDUIManager();

    // 수명주기
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual void PlayerControllerChanged(APlayerController* NewController) override;

    // 외부 공개 API (게임 로직은 이 함수들만 호출)
    UFUNCTION(BlueprintCallable, Category="UI")
    void EnsureMainHUDCreated();

    UFUNCTION(BlueprintCallable, Category="UI")
    void ShowToastMessage(const FText& Message, float DisplaySeconds = 1.2f);

    UFUNCTION(BlueprintCallable, Category="UI")
    void UpdateQuestProgress(const FText& Title, int32 CurrentCount, int32 TargetCount);

    UFUNCTION(BlueprintCallable, Category="UI")
    void SetInteractionPrompt(const FText& PromptText, bool bIsVisible);

private:
    // 메인 HUD 위젯 클래스(WBP 부모) 지정
    UPROPERTY()
    TSubclassOf<UACDPlayerHUDWidget> MainHUDClass;

    // 현재 로컬 플레이어의 HUD 인스턴스
    UPROPERTY(Transient)
    TObjectPtr<UACDPlayerHUDWidget> MainHUD = nullptr;

    // 최근 매핑된 로컬 PlayerController
    TWeakObjectPtr<APlayerController> CachedPlayerController;

    APlayerController* GetPlayerController() const;
};
