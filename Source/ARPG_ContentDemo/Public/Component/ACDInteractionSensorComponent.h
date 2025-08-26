// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACDInteractionSensorComponent.generated.h"

class USphereComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_CONTENTDEMO_API UACDInteractionSensorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UACDInteractionSensorComponent();

	UFUNCTION(BlueprintCallable, Category="Control")
	void ForceUpdate();

	UFUNCTION(BlueprintCallable, Category="Control")
	bool TryInteract(AActor* Instigator);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleOnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
    void HandleOnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	void SetupSensor();
	void UpdateInteractTarget();
	AActor* PickBestInteractable() const;
	void SetCurrentTarget(AActor* NewTarget);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	float SensorRadius = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config", meta=(ClampMin="0.01"))
	float UpdatePeriod = 0.1f;

	UPROPERTY(BlueprintReadOnly, Category="State")
	TWeakObjectPtr<AActor> CurrentTarget;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractTargetChanged, AActor*, NewTarget, const FText&, PromptText);
	UPROPERTY(BlueprintAssignable, Category="Event")
	FOnInteractTargetChanged OnTargetChanged;

private:
	UPROPERTY(Transient)
	USphereComponent* OverlapSensor = nullptr;

	TSet<TWeakObjectPtr<AActor>> Candidates;
	FTimerHandle UpdateHandle;
};
