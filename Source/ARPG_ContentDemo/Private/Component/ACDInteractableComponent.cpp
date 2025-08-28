// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ACDInteractableComponent.h"

UACDInteractableComponent::UACDInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UACDInteractableComponent::CanInteract(AActor* InstigatorActor) const
{
	return !bConsumed;
}

bool UACDInteractableComponent::DoInteract(AActor* InstigatorActor)
{
	if (!CanInteract(InstigatorActor)) return false;

	OnInteracted.Broadcast(InstigatorActor);

	if (bSingleUse)
	{
		bConsumed = true;
	}

	return true;
}
