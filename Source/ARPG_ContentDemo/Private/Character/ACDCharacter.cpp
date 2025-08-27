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
}
