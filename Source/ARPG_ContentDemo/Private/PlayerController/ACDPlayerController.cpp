// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ACDPlayerController.h"
#include "Component/ACDInteractionSensorComponent.h"
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
            OldInteractionSensor->OnTargetChanged.RemoveAll(this);
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

void AACDPlayerController::OnInteractionTargetChanged_Implementation(AActor* NewTarget)
{
    if (HasAuthority() || !IsLocalController())
        return;

    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UACDUIManager* UIManager = LocalPlayer->GetSubsystem<UACDUIManager>())
        {
            UIManager->SetInteractionPrompt(FText::FromString(TEXT("상호작용 타겟변경")), IsValid(NewTarget));
        }
    }
    // 상호작용 컴포넌트 OnRep 내부 등
    //if (IsLocallyControlled())
    //{
        

    // IsLocallyControlled()

    // UI 대응 및 remaining count. 적재.
    // OnChanged로 확인하자.
    //FText Prompt;
    //if (const AActor* TargetActor = GetCurrentTargetActor())
    //{
    //    if (const UACDInteractableComponent* InteractableComponent = TargetActor->FindComponentByClass<UACDInteractableComponent>())
    //    {
    //        Prompt = InteractableComponent->GetPromptText();
    //    }
    //}

    // TODO : UI 대응 필요
    /**
    FText Prompt;
    if (const AActor* TargetActor = GetCurrentTargetActor())
    {
        if (const UACDInteractableComponent* InteractableComponent = TargetActor->FindComponentByClass<UACDInteractableComponent>())
        {
            Prompt = InteractableComponent->GetPromptText();
        }
    }
    */
}