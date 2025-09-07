// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACDInteractableComponent.generated.h"

/*
 * 인터렉터블 컴포넌트 (Interactable Component)
 * - 액터의 상호작용 규칙 정의
 * - CanInteract(): 상호작용 가능한지 여부 확인
 * - DoInteract(): 델리게이트 브로드캐스트
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_CONTENTDEMO_API UACDInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UACDInteractableComponent();

	UFUNCTION(BlueprintPure)
	bool CanInteract(AActor* InstigatorActor) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool DoInteract(AActor* InstigatorActor);

	UFUNCTION(BlueprintPure)
	FText GetPromptText() const;

	UFUNCTION(BlueprintPure)
	FName GetInteractName() const { return InteractName; }

	UFUNCTION(BlueprintPure)
	FText GetInteractAction() const { return InteractAction; }

	UFUNCTION(BlueprintPure)
	int32 GetRemainingUseCount() const { return RemainingUseCount; }

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetInteractName(const FName& InName) { InteractName = InName; }
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetInteractAction(const FText& InAction) { InteractAction = InAction; }

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetUseCount(const int32 UseCount) { RemainingUseCount = UseCount; }

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void BroadcastOnChangedInteractInfo();

	UFUNCTION()
	void OnRep_InteractName();

	UFUNCTION()
	void OnRep_InteractAction();
	
	UFUNCTION()
	void OnRep_RemainingUseCount();

public:
	static const int32 InfiniteCount = INDEX_NONE; // 무한

	// 상호작용 관련 정보 변경 델리게이트
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangedInteractInfo);
	UPROPERTY(BlueprintAssignable)
	FOnChangedInteractInfo OnChangedInteractInfo;

	// 실제 동작 처리 델리게이트
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteracted, AActor*, InstigatorActor);	
	UPROPERTY(BlueprintAssignable)
	FOnInteracted OnInteracted;

private:	
	UPROPERTY(ReplicatedUsing = OnRep_InteractName, EditAnywhere, BlueprintReadOnly, Category="Interactable", Meta = (AllowPrivateAccess = "true"))
	FName InteractName = FName(TEXT(""));

	UPROPERTY(ReplicatedUsing = OnRep_InteractAction, EditAnywhere, BlueprintReadOnly, Category="Interactable", Meta = (AllowPrivateAccess = "true"))
	FText InteractAction = FText::GetEmpty();

	// 남은 상호작용 횟수
	UPROPERTY(ReplicatedUsing = OnRep_RemainingUseCount, EditAnywhere, BlueprintReadOnly, Category="Interactable", Meta = (ClampMin = "-1", AllowPrivateAccess = "true"))
	int32 RemainingUseCount = InfiniteCount;
};
