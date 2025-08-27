// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ACDCharacter.h"
#include "Component/ACDInteractionSensorComponent.h"

AACDCharacter::AACDCharacter()
{
	InteractionSensor = CreateDefaultSubobject<UACDInteractionSensorComponent>(TEXT("InteractionSensor"));
}

void AACDCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (InteractionSensor)
	{
		InteractionSensor->OnTargetChanged.AddDynamic(this, &AACDCharacter::OnInteractionTargetChanged);
	}
}

void AACDCharacter::OnInteractionTargetChanged(AActor* NewTarget, const FText& PromptText)
{
	const bool bShow = (NewTarget != nullptr);
	BP_UpdateInteractionPrompt(bShow, PromptText);
}
