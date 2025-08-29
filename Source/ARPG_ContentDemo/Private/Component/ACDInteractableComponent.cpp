// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ACDInteractableComponent.h"

UACDInteractableComponent::UACDInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// 상호작용 가능한지
bool UACDInteractableComponent::CanInteract(AActor* InstigatorActor) const
{
	return !bConsumed;
}

// OnInteracted 브로드캐스트, 1회성일 경우 bConsumed = true 처리
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
