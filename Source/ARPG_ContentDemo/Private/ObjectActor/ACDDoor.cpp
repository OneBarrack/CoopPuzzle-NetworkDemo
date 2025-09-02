// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectActor/ACDDoor.h"
#include "Component/ACDInteractableComponent.h"
#include "Net/UnrealNetwork.h"

AACDDoor::AACDDoor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

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

void AACDDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AACDDoor, bOpened);
}

void AACDDoor::OnInteracted_Implementation(AActor* InstigatorActor)
{
	bOpened = !bOpened;
	OnChangedDoorStatus.Broadcast(bOpened);

	UE_LOG(LogTemp, Log, TEXT("[%s] Interacted with the door. (%s)"), ANSI_TO_TCHAR(__FUNCTION__), bOpened ? TEXT("Opened") : TEXT("Closed"));
}

void AACDDoor::OnRep_Opened()
{
	OnChangedDoorStatus.Broadcast(bOpened);
}