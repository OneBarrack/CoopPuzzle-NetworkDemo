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
    if (IsValid(MainHUD)) 
    { 
        MainHUD->RemoveFromParent(); 
        MainHUD = nullptr; 
    }

    CachedPlayerController.Reset();
    Super::Deinitialize();
}

void UACDUIManager::PlayerControllerChanged(APlayerController* NewController)
{
    CachedPlayerController = NewController;

    if (!GetWorld() || GetWorld()->IsNetMode(NM_DedicatedServer))
        return;

    EnsureMainHUDCreated(); // 트래블/재소유 후에도 HUD 보장
}

APlayerController* UACDUIManager::GetPlayerController() const
{
    // GetLocalPlayer() ? GetLocalPlayer()->GetPlayerController(GetWorld()) : nullptr;
    return CachedPlayerController.Get();
}

void UACDUIManager::EnsureMainHUDCreated()
{
    if (IsValid(MainHUD) || !IsValid(MainHUDClass)) return;

    if (APlayerController* PC = CachedPlayerController.Get())
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
    EnsureMainHUDCreated();
    MainHUD->ShowToastMessage(Message, DisplaySeconds);
}

void UACDUIManager::UpdateQuestProgress(const FText& Title, int32 CurrentCount, int32 TargetCount)
{
    EnsureMainHUDCreated();
    MainHUD->UpdateQuestProgress(Title, CurrentCount, TargetCount);
}

void UACDUIManager::SetInteractionPrompt(const FText& PromptText, bool bIsVisible)
{
    EnsureMainHUDCreated();
    MainHUD->SetInteractionPrompt(PromptText, bIsVisible);
}

