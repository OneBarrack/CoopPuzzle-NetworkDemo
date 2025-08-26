// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ACDInteractableComponent.h"

UACDInteractableComponent::UACDInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UACDInteractableComponent::DoInteract(AActor* Instigator)
{
	if (CanInteract(Instigator)) return;

	OnInteracted.Broadcast(Instigator);

	if (bSingleUse)
	{
		bConsumed = true;
	}
}
