// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectActor/ACDChest.h"
#include "Component/ACDInteractableComponent.h"

AACDChest::AACDChest()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractableComponent = CreateDefaultSubobject<UACDInteractableComponent>(TEXT("Interactable"));
	InteractableComponent->bSingleUse = true;
}

void AACDChest::BeginPlay()
{
	Super::BeginPlay();
	
	if (ensure(InteractableComponent))
	{
		InteractableComponent->OnInteracted.AddDynamic(this, &AACDChest::OnInteracted);
	}
}

bool AACDChest::CanInteract_Implementation(AActor* InstigatorActor) const
{
	if (IsValid(InteractableComponent))
	{
		return InteractableComponent->CanInteract(InstigatorActor);
	}
	return false;
}

void AACDChest::DoInteract_Implementation(AActor* InstigatorActor)
{
	if (IsValid(InteractableComponent))
	{
		InteractableComponent->DoInteract(InstigatorActor);
	}
}

void AACDChest::OnInteracted_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("[%s] Chest opened"), ANSI_TO_TCHAR(__FUNCTION__));
}
