// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ACDInteractionInterface.h"
#include "Common/ACDStruct.h"
#include "ACDDoor.generated.h"

class UACDInteractableComponent;

/**
 * 상호작용 액터
 * - IInteractionInterface 구현
 * - 내부적으로 UACDInteractableComponent 생성
 * - OnInteracted에서 상태 토글 및 델리게이트 브로드캐스트, 실제 연출은 BP에서 구현
 */
UCLASS()
class ARPG_CONTENTDEMO_API AACDDoor : public AActor, public IACDInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AACDDoor();

protected:
	virtual void BeginPlay() override;

	// Interaction interface
	virtual bool CanInteract_Implementation(AActor* InstigatorActor) const override;
	virtual void DoInteract_Implementation(AActor* InstigatorActor) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool HasEnoughItemsForUnlocked(AActor* InstigatorActor) const;
	bool ConsumeItemsForUnlocked(AActor* InstigatorActor) const;

	UFUNCTION(BlueprintNativeEvent)
	void OnInteracted(AActor* InstigatorActor);
	virtual void OnInteracted_Implementation(AActor* InstigatorActor);

	UFUNCTION()
	void OnRep_Opened();

	UFUNCTION()
	void OnRep_IsLocked();

public:
	UPROPERTY(ReplicatedUsing = OnRep_Opened, EditAnywhere, BlueprintReadWrite, Category="Interact")
	bool bOpened = false;

	UPROPERTY(ReplicatedUsing = OnRep_IsLocked, EditAnywhere, BlueprintReadWrite, Category="Interact|Lock")
	bool bIsLocked = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Interact|Lock")
	FACDRewardItem KeyItemInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interact")
	TObjectPtr<UACDInteractableComponent> InteractableComponent = nullptr;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangedDoorStatus, bool, bOpened, bool, bIsLocked);
	UPROPERTY(BlueprintAssignable)
	FOnChangedDoorStatus OnChangedDoorStatus;
};
