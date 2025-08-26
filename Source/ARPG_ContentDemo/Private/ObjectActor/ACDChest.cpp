// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectActor/ACDChest.h"
#include "Component/ACDInteractableComponent.h"

AACDChest::AACDChest()
{
	PrimaryActorTick.bCanEverTick = false;

	Interactable = CreateDefaultSubobject<UACDInteractableComponent>(TEXT("Interactable"));
	Interactable->bSingleUse = true;
}

void AACDChest::BeginPlay()
{
	Super::BeginPlay();
	check(Interactable != nullptr);
	Interactable->OnInteracted.AddDynamic(this, &AACDChest::OnInteracted);
}

void AACDChest::OnInteracted(AActor* InstigatorActor)
{
	UE_LOG(LogTemp, Log, TEXT("[%s] Chest opened"), ANSI_TO_TCHAR(__FUNCTION__));
}
