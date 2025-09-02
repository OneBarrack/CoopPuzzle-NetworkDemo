// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ACDPlayerController.h"
#include "Component/ACDInteractionSensorComponent.h"
#include "Component/ACDInteractableComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Blueprint/UserWidget.h"
#include "Manager/ACDUIManager.h"

void AACDPlayerController::BeginPlay()
{
    Super::BeginPlay();

    AddContext(GameplayContext, 0);
    BindToPawnSensor(GetPawn());
}

void AACDPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (auto* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (ToggleUIAction)
        {
            EnhancedInput->BindAction(ToggleUIAction, ETriggerEvent::Started, this, &AACDPlayerController::ToggleUIMode);
        }
    }
}

void AACDPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    
    BindToPawnSensor(InPawn);
}

void AACDPlayerController::OnUnPossess()
{
    BindToPawnSensor(nullptr);
    RemoveContext(UIContext);
    
    Super::OnUnPossess();
}

void AACDPlayerController::BindToPawnSensor(APawn* NewPawn)
{
    // 기존 바인딩 해제
    if (APawn* Old = GetPawn())
    {
        if (UACDInteractionSensorComponent* OldInteractionSensor = Old->FindComponentByClass<UACDInteractionSensorComponent>())
        {
            OldInteractionSensor->OnTargetChanged.RemoveDynamic(this, &AACDPlayerController::OnInteractionTargetChanged);
        }
    }

    // 새 폰 바인딩
    if (IsValid(NewPawn))
    {
        if (UACDInteractionSensorComponent* NewInteractionSensor = NewPawn->FindComponentByClass<UACDInteractionSensorComponent>())
        {
            NewInteractionSensor->OnTargetChanged.AddUniqueDynamic(this , &AACDPlayerController::OnInteractionTargetChanged);
        }
    }
}

UEnhancedInputLocalPlayerSubsystem* AACDPlayerController::GetInputSubsystem() const
{
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        return ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
    }
    return nullptr;
}

void AACDPlayerController::AddContext(UInputMappingContext* Context, int32 Priority) const
{
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetInputSubsystem(); Subsystem && Context)
    {
        Subsystem->AddMappingContext(Context, Priority);
    }
}

void AACDPlayerController::RemoveContext(UInputMappingContext* Context) const
{
    if (auto* Subsystem = GetInputSubsystem(); Subsystem && Context)
    {
        Subsystem->RemoveMappingContext(Context);
    }
}

void AACDPlayerController::UpdateInteractionTargetUIInfo()
{
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UACDUIManager* UIManager = LocalPlayer->GetSubsystem<UACDUIManager>())
        {
            FText PromptText = FText::GetEmpty();
            const bool bIsVisible = InteractionTarget.IsValid();

            if (bIsVisible)
            {
                if (UACDInteractableComponent* InteractableComponent = InteractionTarget.Get()->FindComponentByClass<UACDInteractableComponent>())
                {
                    PromptText = InteractableComponent->GetPromptText();
                }
            }

            UIManager->SetInteractionPrompt(PromptText, bIsVisible);
        }
    }
}

void AACDPlayerController::ToggleUIMode()
{
    bUIMode = !bUIMode;

    if (bUIMode)
    {
        AddContext(UIContext, 10);

        FInputModeGameAndUI Mode;
        Mode.SetHideCursorDuringCapture(false);
        SetInputMode(Mode);

        bShowMouseCursor = true;
    }
    else
    {
        RemoveContext(UIContext);

        FInputModeGameOnly Mode;
        SetInputMode(Mode);

        bShowMouseCursor = false;
    }
}

// 상호작용 타겟 변경 및 타겟 상태 변화 델리게이트에 연결
void AACDPlayerController::OnInteractionTargetChanged_Implementation(AActor* NewTarget)
{
    if (!IsLocalController()) 
        return;

    if (InteractionTarget.IsValid())
    {
        if (UACDInteractableComponent* InteractableComponent = InteractionTarget.Get()->FindComponentByClass<UACDInteractableComponent>())
        {
            InteractableComponent->OnChangedInteractInfo.RemoveAll(this);            
        }
    }

    if (IsValid(NewTarget))
    {
        if (UACDInteractableComponent* InteractableComponent = NewTarget->FindComponentByClass<UACDInteractableComponent>())
        {
            InteractableComponent->OnChangedInteractInfo.AddUniqueDynamic(this, &AACDPlayerController::UpdateInteractionTargetUIInfo);
        }
    }

    InteractionTarget = NewTarget;
    UpdateInteractionTargetUIInfo();
}