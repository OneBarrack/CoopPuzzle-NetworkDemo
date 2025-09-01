// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACDInteractionSensorComponent.generated.h"

class USphereComponent;

/**
 * 인터렉션 센서 컴포넌트 (Interaction Sensor Component)
 * - 내부적으로 Sphere Subobject를 사용하여 주변 상호작용 후보를 Overlap으로 수집
 * - 일정 주기(UpdatePeriod)마다 후보 중 타깃을 선택(PickBest…)하고,
 *   OnTargetChanged 델리게이트로 UI에 알림
 * - ForceUpdate()는 입력 직전 즉시 재평가를 가능하게 하여 반응성을 높임
 */
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

	UFUNCTION(BlueprintPure, Category="Interaction")
	AActor* GetCurrentTargetActor() const { return CurrentTargetActor; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRegister() override;
	virtual void OnUnregister() override;

	UFUNCTION()
	void HandleOnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
    void HandleOnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnRep_CurrentTargetActor();

private:
	void UpdateInteractTarget();
	AActor* PickBestInteractable() const;
	void SetCurrentTarget(AActor* NewTargetActor);

public:
	UPROPERTY(transient)
	TObjectPtr<USphereComponent> SensorSphere = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Sensor")
	float SensorRadius = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Sensor", meta=(ClampMin="0.01"))
	float UpdatePeriod = 0.1f;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractTargetChanged, AActor*, NewTarget);
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnInteractTargetChanged OnTargetChanged;

private:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentTargetActor, Transient)
	TObjectPtr<AActor> CurrentTargetActor;

	TSet<TWeakObjectPtr<AActor>> Candidates;
	FTimerHandle UpdateHandle;
};
