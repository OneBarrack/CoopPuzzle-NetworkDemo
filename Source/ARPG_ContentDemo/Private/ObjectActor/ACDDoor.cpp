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

	if (IsValid(InteractableComponent))
	{
		InteractableComponent->OnInteracted.AddDynamic(this, &AACDDoor::OnInteracted);
	}
}

// 인터렉션 가능 여부 체크 - InteractableComponent에 위임
bool AACDDoor::CanInteract_Implementation(AActor* InstigatorActor) const
{
	if (IsValid(InteractableComponent))
	{
		return InteractableComponent->CanInteract(InstigatorActor);
	}
	return false;
}

// 인터렉션 실행 - InteractableComponent에 위임 + 연출 트리거
void AACDDoor::DoInteract_Implementation(AActor* InstigatorActor)
{
	if (IsValid(InteractableComponent))
	{
		InteractableComponent->DoInteract(InstigatorActor);
	}
}

void AACDDoor::OnInteracted_Implementation(AActor* InstigatorActor)
{
	bOpened = !bOpened;
	OnChangedDoorStatus.Broadcast(bOpened);

	UE_LOG(LogTemp, Log, TEXT("[%s] Interacted with the door. (%s)"), ANSI_TO_TCHAR(__FUNCTION__), bOpened ? TEXT("Opened") : TEXT("Closed"));
}
