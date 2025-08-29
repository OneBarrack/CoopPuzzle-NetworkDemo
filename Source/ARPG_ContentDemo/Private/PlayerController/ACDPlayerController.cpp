// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ACDPlayerController.h"
#include "Component/ACDInteractionSensorComponent.h"
#include "Blueprint/UserWidget.h"

void AACDPlayerController::BeginPlay()
{
    Super::BeginPlay();

    BindToPawnSensor(GetPawn());
}

void AACDPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    BindToPawnSensor(InPawn);
}

void AACDPlayerController::OnUnPossess()
{
    BindToPawnSensor(nullptr);
    Super::OnUnPossess();
}

void AACDPlayerController::BindToPawnSensor(APawn* InPawn)
{
    // 기존 바인딩 해제
    if (APawn* Old = GetPawn())
    {
        if (UACDInteractionSensorComponent* OldSensor = Old->FindComponentByClass<UACDInteractionSensorComponent>())
        {
            OldSensor->OnTargetChanged.RemoveAll(this);
        }
    }

    // 새 폰 바인딩
    if (IsValid(InPawn))
    {
        if (UACDInteractionSensorComponent* Sensor = InPawn->FindComponentByClass<UACDInteractionSensorComponent>())
        {
            Sensor->OnTargetChanged.AddUniqueDynamic(this , &AACDPlayerController::OnInteractionTargetChanged);
        }
    }
}

void AACDPlayerController::OnInteractionTargetChanged_Implementation(AActor* NewTarget , const FText& PromptText)
{

}