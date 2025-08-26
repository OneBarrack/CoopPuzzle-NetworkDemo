// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectActor/ACDLever.h"
#include "Component/ACDInteractableComponent.h"

AACDLever::AACDLever()
{
	PrimaryActorTick.bCanEverTick = false;

	Interactable = CreateDefaultSubobject<UACDInteractableComponent>(TEXT("Interactable"));
	Interactable->bSingleUse = true;
}

void AACDLever::BeginPlay()
{
	Super::BeginPlay();
	check(Interactable != nullptr);
	Interactable->OnInteracted.AddDynamic(this, &AACDLever::OnInteracted);
}

void AACDLever::OnInteracted(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("[%s] Lever pulled."), ANSI_TO_TCHAR(__FUNCTION__));
}

