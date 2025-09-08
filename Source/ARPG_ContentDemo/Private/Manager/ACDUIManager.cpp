// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/ACDUIManager.h"
#include "Settings/ACDGameDataSettings.h"
#include "UI/ACDPlayerHUDWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

UACDUIManager::UACDUIManager()
{
}

void UACDUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    const UACDGameDataSettings* Settings = GetDefault<UACDGameDataSettings>();
    if (Settings && !Settings->PlayerHUDClass.IsNull())
    {
        MainHUDClass = Settings->PlayerHUDClass.LoadSynchronous();
    }
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

// HUD 보장
void UACDUIManager::EnsureMainHUDCreated()
{
    if (IsValid(MainHUD) || !IsValid(MainHUDClass)) return;

    if (APlayerController* PC = CachedPlayerController.Get())
    {
        MainHUD = CreateWidget<UACDPlayerHUDWidget>(PC, MainHUDClass);
        if (IsValid(MainHUD))
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

void UACDUIManager::OpenInventory()
{
    EnsureMainHUDCreated();
    MainHUD->OpenInventory();
}

