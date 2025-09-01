// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ACDCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Component/ACDInteractionSensorComponent.h"
#include "Interface/ACDInteractionInterface.h"

AACDCharacter::AACDCharacter()
{
	InteractionSensor = CreateDefaultSubobject<UACDInteractionSensorComponent>(TEXT("InteractionSenSor"));
    InteractionSensor->SetIsReplicated(true);
}

void AACDCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AACDCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (auto* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (InteractAction)
        {
            EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &AACDCharacter::Interact);
        }
    }
}

void AACDCharacter::Interact()
{
    if (IsValid(InteractionSensor))
    {
        InteractionSensor->TryInteract();
    }
}
