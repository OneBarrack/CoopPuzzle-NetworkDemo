// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ACDCharacter.h"
#include "Component/ACDInteractionSensorComponent.h"

// Sets default values
AACDCharacter::AACDCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AACDCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AACDCharacter::OnTargetChanged(AActor* NewTarget, const FText& PromptText)
{
	const bool bShow = (NewTarget != nullptr);
	BP_UpdateInteractPrompt(bShow, PromptText);
}
