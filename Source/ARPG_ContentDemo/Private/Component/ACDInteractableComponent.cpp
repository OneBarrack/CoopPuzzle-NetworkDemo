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

	DOREPLIFETIME(UACDInteractableComponent, InteractName);
	DOREPLIFETIME(UACDInteractableComponent, InteractAction);
	DOREPLIFETIME(UACDInteractableComponent, RemainingUseCount);
}

bool UACDInteractableComponent::CanInteract(AActor* InstigatorActor) const
{
	return (RemainingUseCount > 0) || (RemainingUseCount == InfiniteCount);
}

bool UACDInteractableComponent::DoInteract(AActor* InstigatorActor)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		if (CanInteract(InstigatorActor))
		{
			if (RemainingUseCount > 0)
			{
				--RemainingUseCount;
			}

			Multicast_BroadcastOnInteracted(InstigatorActor);

			return true;
		}
	}

	return false;
}

FText UACDInteractableComponent::GetPromptText() const
{
	return FText::Format(FText::FromString(TEXT("{0} {1}")), InteractAction, FText::FromName(InteractName));;
}

void UACDInteractableComponent::Multicast_BroadcastOnInteracted_Implementation(AActor* InstigatorActor)
{
	OnInteracted.Broadcast(InstigatorActor);
}

void UACDInteractableComponent::BroadcastOnChangedInteractInfo()
{
	OnChangedInteractInfo.Broadcast();
}

void UACDInteractableComponent::OnRep_InteractName()
{
	BroadcastOnChangedInteractInfo();
}

void UACDInteractableComponent::OnRep_InteractAction()
{
	BroadcastOnChangedInteractInfo();
}

void UACDInteractableComponent::OnRep_RemainingUseCount()
{
	BroadcastOnChangedInteractInfo();
}
