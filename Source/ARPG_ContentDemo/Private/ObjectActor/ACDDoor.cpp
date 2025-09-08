// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectActor/ACDDoor.h"
#include "Component/ACDInteractableComponent.h"
#include "PlayerState/ACDPlayerState.h"
#include "Inventory/ACDInventoryComponent.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogACDDoor, Log, All);

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
		InteractableComponent->OnInteracted.AddUniqueDynamic(this, &AACDDoor::OnInteracted);
	}
}

// 잠금해제 가능여부
bool AACDDoor::HasEnoughItemsForUnlocked(AActor* InstigatorActor) const
{
	if (!bIsLocked) return true;

	if (bIsLocked && KeyItemInfo.ItemID > 0 && KeyItemInfo.Count > 0)
	{
		if (APawn* Pawn = Cast<APawn>(InstigatorActor))
		{
			if (AACDPlayerState* PS = Pawn->GetPlayerState<AACDPlayerState>())
			{
				if (UACDInventoryComponent* InventoryComponent = PS->FindComponentByClass<UACDInventoryComponent>())
				{
					const FACDInventoryList& InventoryList = InventoryComponent->GetInventory();
					const TArray<FACDInventoryItem>& InventoryItems = InventoryList.GetItems();

					const int32 MatchedItemIndex = InventoryList.FindIndex(KeyItemInfo.ItemID);
					if (MatchedItemIndex != INDEX_NONE && InventoryItems[MatchedItemIndex].Quantity >= KeyItemInfo.Count)
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

// 잠금해제 아이템 소모
bool AACDDoor::ConsumeItemsForUnlocked(AActor* InstigatorActor) const
{
	if (bIsLocked && KeyItemInfo.ItemID > 0 && KeyItemInfo.Count > 0)
	{
		if (APawn* Pawn = Cast<APawn>(InstigatorActor))
		{
			if (AACDPlayerState* PS = Pawn->GetPlayerState<AACDPlayerState>())
			{
				if (UACDInventoryComponent* InventoryComponent = PS->FindComponentByClass<UACDInventoryComponent>())
				{
					InventoryComponent->ConsumeItem(KeyItemInfo.ItemID, KeyItemInfo.Count);

					UE_LOG(LogACDDoor, Log, TEXT("[%s] Consumed items for unlocked itemId(%d) Count(%d)"), ANSI_TO_TCHAR(__FUNCTION__), KeyItemInfo.ItemID, KeyItemInfo.Count);

					return true;
				}
			}
		}
	}

	return false;
}

// 인터렉션 가능 여부 체크 - InteractableComponent에 위임
bool AACDDoor::CanInteract_Implementation(AActor* InstigatorActor) const
{
	if (IsValid(InteractableComponent))
	{
		return (InteractableComponent->CanInteract(InstigatorActor) && HasEnoughItemsForUnlocked(InstigatorActor));
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
	DOREPLIFETIME(AACDDoor, bIsLocked);
	DOREPLIFETIME(AACDDoor, KeyItemInfo);
}

void AACDDoor::OnInteracted_Implementation(AActor* InstigatorActor)
{
	if (!HasAuthority() || !IsValid(InstigatorActor)) return;

	if (ConsumeItemsForUnlocked(InstigatorActor))
	{
		bIsLocked = false;
	}

	bOpened = !bOpened;
	OnChangedDoorStatus.Broadcast(bOpened, bIsLocked);

	UE_LOG(LogACDDoor, Log, TEXT("[%s] Interacted. Door was (%s)"), ANSI_TO_TCHAR(__FUNCTION__), bOpened ? TEXT("opened") : TEXT("closed"));
}

void AACDDoor::OnRep_Opened()
{
	OnChangedDoorStatus.Broadcast(bOpened, bIsLocked);
}

void AACDDoor::OnRep_IsLocked()
{
	OnChangedDoorStatus.Broadcast(bOpened, bIsLocked);
}
