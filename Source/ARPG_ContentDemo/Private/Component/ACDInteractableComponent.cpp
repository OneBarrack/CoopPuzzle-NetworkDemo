// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ACDInteractableComponent.h"
#include "Net/UnrealNetwork.h"

UACDInteractableComponent::UACDInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UACDInteractableComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UACDInteractableComponent, RemainingUseCount);
}

// 상호작용 가능한지
bool UACDInteractableComponent::CanInteract(AActor* InstigatorActor) const
{
	return (RemainingUseCount > 0) || (RemainingUseCount == InfiniteCount);
}

// OnInteracted 브로드캐스트, 1회성일 경우 bConsumed = true 처리
bool UACDInteractableComponent::DoInteract(AActor* InstigatorActor)
{
	if (!GetOwner()) return false;
	
	Server_DoInteract(InstigatorActor);

	return true;
}

void UACDInteractableComponent::Server_DoInteract_Implementation(AActor* InstigatorActor)
{
	if (!CanInteract(InstigatorActor)) return;

	if (RemainingUseCount > 0)
	{
		--RemainingUseCount;
	}

	Multicast_BroadcastOnInteracted(InstigatorActor);
}

void UACDInteractableComponent::Multicast_BroadcastOnInteracted_Implementation(AActor* InstigatorActor)
{
	OnInteracted.Broadcast(InstigatorActor);
}

void UACDInteractableComponent::OnRep_RemainingUseCount()
{
	OnUpdatedRemainingUseCount.Broadcast();
}
