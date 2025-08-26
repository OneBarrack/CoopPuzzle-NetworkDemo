// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectActor/ACDDoor.h"
#include "Component/ACDInteractableComponent.h"

AACDDoor::AACDDoor()
{
	PrimaryActorTick.bCanEverTick = false;

	Interactable = CreateDefaultSubobject<UACDInteractableComponent>(TEXT("Interactable"));
	Interactable->bSingleUse = true;
}

void AACDDoor::BeginPlay()
{
	Super::BeginPlay();
	check(Interactable != nullptr);
	Interactable->OnInteracted.AddDynamic(this, &AACDDoor::OnInteracted);
}

void AACDDoor::OnInteracted(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("[%s] Door opened."), ANSI_TO_TCHAR(__FUNCTION__));
}
