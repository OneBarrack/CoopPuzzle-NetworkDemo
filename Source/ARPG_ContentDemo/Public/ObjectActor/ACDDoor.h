// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACDDoor.generated.h"

class UACDInteractableComponent;

UCLASS()
class ARPG_CONTENTDEMO_API AACDDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	AACDDoor();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnInteracted(AActor* InstigatorActor);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interact")
	UACDInteractableComponent* Interactable = nullptr;
};
