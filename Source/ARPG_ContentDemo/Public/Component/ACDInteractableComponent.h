// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACDInteractableComponent.generated.h"

/*
 * 인터렉터블 컴포넌트 (Interactable Component)
 * - 액터의 상호작용 규칙 정의
 * - PromptText, bSingleUse, bConsumed 상태를 보관
 * - CanInteract(): 상호작용 가능한지 여부 확인
 * - DoInteract(): 델리게이트 브로드캐스트 후 멱등 처리
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_CONTENTDEMO_API UACDInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UACDInteractableComponent();

	UFUNCTION()
	bool CanInteract(AActor* InstigatorActor) const;

	UFUNCTION()
	bool DoInteract(AActor* InstigatorActor);

	UFUNCTION(BlueprintPure)
	FText GetPromptText() const { return PromptText; }

	UFUNCTION(BlueprintPure)
	int32 GetRemainingUseCount() const { return RemainingUseCount; }

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetPromptText(const FText& NewPromptText) { PromptText = NewPromptText; }

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetUseCount(const int32 UseCount) { RemainingUseCount = UseCount; }

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 서버 전용 실제 처리
    UFUNCTION(Server, Reliable)
    void Server_DoInteract(AActor* InstigatorActor);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BroadcastOnInteracted(AActor* InstigatorActor);
	
	UFUNCTION()
	void OnRep_RemainingUseCount();

public:
	static const int32 InfiniteCount = INDEX_NONE; // 무한

	// 남은 동작 횟수 델리게이트
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdatedRemainingUseCount);
	UPROPERTY(BlueprintAssignable)
	FOnUpdatedRemainingUseCount OnUpdatedRemainingUseCount;

	// 실제 동작 처리 델리게이트
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteracted, AActor*, InstigatorActor);	
	UPROPERTY(BlueprintAssignable)
	FOnInteracted OnInteracted;

private:
	// HUD에 표시할 프롬프트 텍스트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interactable", Meta = (AllowPrivateAccess = "true"))
	FText PromptText = FText::GetEmpty();

	// 남은 상호작용 횟수
	UPROPERTY(ReplicatedUsing = OnRep_RemainingUseCount, EditAnywhere, BlueprintReadOnly, Category="State", Meta = (ClampMin = "-1", AllowPrivateAccess = "true"))
	int32 RemainingUseCount = InfiniteCount;
};
