// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectActor/ACDChest.h"
#include "Component/ACDInteractableComponent.h"
#include "PlayerState/ACDPlayerState.h"
#include "Manager/ACDItemManager.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogACDChest, Log, All);

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
		InteractableComponent->OnInteracted.AddUniqueDynamic(this, &AACDChest::OnInteracted);
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

void AACDChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AACDChest, bOpened);
}

void AACDChest::OnInteracted_Implementation(AActor* InstigatorActor)
{
	if (!HasAuthority() || !IsValid(InstigatorActor)) return;

	bOpened = !bOpened;
	OnChangedChestStatus.Broadcast(bOpened);

	if (bOpened && !RewardPackRow.IsNone())
	{
		if (APawn* Pawn = Cast<APawn>(InstigatorActor))
		{
			if (AACDPlayerState* PlayerState = Pawn->GetPlayerState<AACDPlayerState>())
			{
				if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
				{
					if (UACDItemManager* ItemManager = GameInstance->GetSubsystem<UACDItemManager>())
					{
						ItemManager->GrantReward(RewardPackRow, PlayerState);
					}
				}
			}
		}
	}

	UE_LOG(LogACDChest, Log, TEXT("[%s] Interacted. Chest was %s"), ANSI_TO_TCHAR(__FUNCTION__), (bOpened ? TEXT("opened") : TEXT("closed")));
}

void AACDChest::OnRep_Opened()
{
	OnChangedChestStatus.Broadcast(bOpened);
}
