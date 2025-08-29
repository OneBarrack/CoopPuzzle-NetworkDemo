// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/ACDUIManager.h"
#include "UI/HUD/ACDPlayerHUDWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

UACDUIManager::UACDUIManager()
{
    static ConstructorHelpers::FClassFinder<UACDPlayerHUDWidget> HudBPClass(TEXT("/Game/Blueprints/UI/WBP_PlayerHUD"));
    if (HudBPClass.Succeeded())
    {
        MainHUDClass = HudBPClass.Class;
    }
}

void UACDUIManager::Initialize(FSubsystemCollectionBase&) 
{

}

void UACDUIManager::Deinitialize()
{
    MainHUD = nullptr;
    CachedPlayerController.Reset();
}

void UACDUIManager::PlayerControllerChanged(APlayerController* NewController)
{
    CachedPlayerController = NewController;
    EnsureMainHUDCreated(); // 트래블/재소유 후에도 HUD 보장
}

APlayerController* UACDUIManager::GetPlayerController() const
{
    return GetLocalPlayer() ? GetLocalPlayer()->GetPlayerController(GetWorld()) : nullptr;
}

void UACDUIManager::EnsureMainHUDCreated()
{
    if (IsValid(MainHUD) || !IsValid(MainHUDClass)) return;

    if (APlayerController* PC = GetPlayerController())
    {
        MainHUD = CreateWidget<UACDPlayerHUDWidget>(PC, MainHUDClass);
        if (MainHUD)
        {
            MainHUD->AddToPlayerScreen(/*ZOrder*/0);
            MainHUD->SetVisibility(ESlateVisibility::HitTestInvisible);
            MainHUD->ShowToastMessage(FText::FromString(TEXT("HUD Ready")), 0.8f);
        }
    }
}

void UACDUIManager::ShowToastMessage(const FText& Message, float DisplaySeconds)
{
    if (IsValid(MainHUD))
    { 
        MainHUD->ShowToastMessage(Message, DisplaySeconds); 
    }
}

void UACDUIManager::UpdateQuestProgress(const FText& Title, int32 CurrentCount, int32 TargetCount)
{
    if (IsValid(MainHUD))
    { 
        MainHUD->UpdateQuestProgress(Title, CurrentCount, TargetCount);
    }
}

void UACDUIManager::SetInteractionPrompt(const FText& PromptText, bool bIsVisible)
{
    if (IsValid(MainHUD))
    { 
        MainHUD->SetInteractionPrompt(PromptText, bIsVisible);
    }
}

