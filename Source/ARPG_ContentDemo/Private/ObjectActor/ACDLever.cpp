// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectActor/ACDLever.h"
#include "Component/ACDInteractableComponent.h"

AACDLever::AACDLever()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractableComponent = CreateDefaultSubobject<UACDInteractableComponent>(TEXT("Interactable"));
	InteractableComponent->SetSingleUse(true);
}

void AACDLever::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(InteractableComponent))
	{
		InteractableComponent->OnInteracted.AddDynamic(this, &AACDLever::OnInteracted);
	}
}

// 인터렉션 가능 여부 체크 - InteractableComponent에 위임
bool AACDLever::CanInteract_Implementation(AActor* InstigatorActor) const
{
	if (IsValid(InteractableComponent))
	{
		return InteractableComponent->CanInteract(InstigatorActor);
	}
	return false;
}

// 인터렉션 실행 - InteractableComponent에 위임 + 연출 트리거
void AACDLever::DoInteract_Implementation(AActor* InstigatorActor)
{
	if (IsValid(InteractableComponent))
	{
		InteractableComponent->DoInteract(InstigatorActor);
	}
}

void AACDLever::OnInteracted_Implementation(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("[%s] Lever pulled."), ANSI_TO_TCHAR(__FUNCTION__));
}

