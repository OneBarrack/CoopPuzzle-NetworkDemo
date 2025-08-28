// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ACDInteractionInterface.h"
#include "ACDDoor.generated.h"

class UACDInteractableComponent;

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

	UFUNCTION(BlueprintNativeEvent)
	void OnInteracted(AActor* InstigatorActor);
	virtual void OnInteracted_Implementation(AActor* InstigatorActor);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interact")
	TObjectPtr<UACDInteractableComponent> InteractableComponent = nullptr;
};
