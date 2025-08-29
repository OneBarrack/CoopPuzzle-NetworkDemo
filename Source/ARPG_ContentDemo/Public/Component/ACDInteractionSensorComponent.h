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

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UFUNCTION(BlueprintCallable, Category="Control")
	void ForceUpdate();

	UFUNCTION(BlueprintCallable, Category="Control")
	bool TryInteract(AActor* Instigator);

	UFUNCTION(BlueprintPure, Category="Interaction")
	AActor* GetCurrentTargetActor() const { return CurrentTargetActor.Get(); }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void HandleOnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
    void HandleOnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	void UpdateInteractTarget();
	AActor* PickBestInteractable() const;
	void SetCurrentTarget(AActor* NewTargetActor);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	float SensorRadius = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config", meta=(ClampMin="0.01"))
	float UpdatePeriod = 0.1f;

	UPROPERTY()
	TObjectPtr<USphereComponent> SensorSphere;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractTargetChanged, AActor*, NewTarget, const FText&, PromptText);
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnInteractTargetChanged OnTargetChanged;

private:
	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> CurrentTargetActor;

	TSet<TWeakObjectPtr<AActor>> Candidates;
	FTimerHandle UpdateHandle;
};
