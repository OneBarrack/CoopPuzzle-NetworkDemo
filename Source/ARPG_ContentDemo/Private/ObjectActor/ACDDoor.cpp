// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectActor/ACDDoor.h"
#include "Component/ACDInteractableComponent.h"

AACDDoor::AACDDoor()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractableComponent = CreateDefaultSubobject<UACDInteractableComponent>(TEXT("Interactable"));
}

void AACDDoor::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(InteractableComponent))
	{
		InteractableComponent->OnInteracted.AddDynamic(this, &AACDDoor::OnInteracted);
	}
}

bool AACDDoor::CanInteract_Implementation(AActor* InstigatorActor) const
{
	if (IsValid(InteractableComponent))
	{
		return InteractableComponent->CanInteract(InstigatorActor);
	}
	return false;
}

void AACDDoor::DoInteract_Implementation(AActor* InstigatorActor)
{
	if (IsValid(InteractableComponent))
	{
		InteractableComponent->DoInteract(InstigatorActor);
	}
}

void AACDDoor::OnInteracted_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("[%s] Interacted with the door."), ANSI_TO_TCHAR(__FUNCTION__));
}
