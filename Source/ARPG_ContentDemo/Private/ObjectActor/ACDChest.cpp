// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectActor/ACDChest.h"
#include "Component/ACDInteractableComponent.h"

AACDChest::AACDChest()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	InteractableComponent = CreateDefaultSubobject<UACDInteractableComponent>(TEXT("Interactable"));
}

void AACDChest::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(InteractableComponent))
	{
		InteractableComponent->OnInteracted.AddDynamic(this, &AACDChest::OnInteracted);
	}
}

// 인터렉션 가능 여부 체크 - InteractableComponent에 위임
bool AACDChest::CanInteract_Implementation(AActor* InstigatorActor) const
{
	if (IsValid(InteractableComponent))
	{
		return InteractableComponent->CanInteract(InstigatorActor);
	}
	return false;
}

// 인터렉션 실행 - InteractableComponent에 위임 + 연출 트리거
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
