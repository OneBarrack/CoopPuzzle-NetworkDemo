// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectActor/ACDLever.h"
#include "Component/ACDInteractableComponent.h"
#include "Net/UnrealNetwork.h"

AACDLever::AACDLever()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	InteractableComponent = CreateDefaultSubobject<UACDInteractableComponent>(TEXT("Interactable"));
}

void AACDLever::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(InteractableComponent))
	{
		InteractableComponent->OnInteracted.AddUniqueDynamic(this, &AACDLever::OnInteracted);
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

void AACDLever::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AACDLever, bIsOn);
}

void AACDLever::OnInteracted_Implementation(AActor* InstigatorActor)
{
	bIsOn = !bIsOn;
	OnChangedLeverStatus.Broadcast(bIsOn);

	UE_LOG(LogTemp, Log, TEXT("[%s] Lever was activated. (%s)"), ANSI_TO_TCHAR(__FUNCTION__), bIsOn ? TEXT("ON") : TEXT("OFF"));
}

void AACDLever::OnRep_IsOn()
{
	OnChangedLeverStatus.Broadcast(bIsOn);
}