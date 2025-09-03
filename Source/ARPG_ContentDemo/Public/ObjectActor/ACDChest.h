// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ACDInteractionInterface.h"
#include "ACDChest.generated.h"

class UACDInteractableComponent;

/**
 * 상호작용 액터
 * - IInteractionInterface 구현
 * - 내부적으로 UACDInteractableComponent 생성
 * - OnInteracted에서 상태 토글 및 델리게이트 브로드캐스트, 실제 연출은 BP에서 구현
 */
UCLASS()
class ARPG_CONTENTDEMO_API AACDChest : public AActor, public IACDInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AACDChest();

protected:
	virtual void BeginPlay() override;

	// Interaction interface
	virtual bool CanInteract_Implementation(AActor* InstigatorActor) const override;
	virtual void DoInteract_Implementation(AActor* InstigatorActor) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintNativeEvent)
	void OnInteracted(AActor* InstigatorActor);
	virtual void OnInteracted_Implementation(AActor* InstigatorActor);

	UFUNCTION()
	void OnRep_Opened();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interact")
	TObjectPtr<UACDInteractableComponent> InteractableComponent = nullptr;

	UPROPERTY(ReplicatedUsing = OnRep_Opened, EditAnywhere, BlueprintReadWrite, Category="Interact")
	bool bOpened = false;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedChestStatus, bool, bOpened);
	UPROPERTY(BlueprintAssignable)
	FOnChangedChestStatus OnChangedChestStatus;
};