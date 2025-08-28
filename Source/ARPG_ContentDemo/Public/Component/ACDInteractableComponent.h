// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACDInteractableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_CONTENTDEMO_API UACDInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UACDInteractableComponent();

public:
	UFUNCTION()
	bool CanInteract(AActor* InstigatorActor) const;

	UFUNCTION()
	bool DoInteract(AActor* InstigatorActor);

public:
	// HUD에 표시할 프롬프트 텍스트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interactable")
	FText PromptText = FText::FromString(TEXT("Interact"));

	// 1회성 상호작용 여부(상자/일회성 레버 등)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="State")
	bool bSingleUse = false;

	// 이미 사용되었는지(런타임 상태)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	bool bConsumed = false;

	// 실제 동작 처리 델리게이트 바인딩 
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteracted, AActor*, InstigatorActor);	
	UPROPERTY(BlueprintAssignable)
	FOnInteracted OnInteracted;
};
