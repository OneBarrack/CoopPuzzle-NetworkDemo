// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ACDInteractionSensorComponent.h"
#include "Component/ACDInteractableComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

UACDInteractionSensorComponent::UACDInteractionSensorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UACDInteractionSensorComponent::BeginPlay()
{
	Super::BeginPlay();
	SetupSensor();

	if (UpdatePeriod > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			UpdateHandle, this, &UACDInteractionSensorComponent::UpdateInteractTarget,
			UpdatePeriod, true);
	}
}

void UACDInteractionSensorComponent::SetupSensor()
{	
	if (!IsValid(GetOwner()))
	{
		return;
	}

	OverlapSensor = NewObject<USphereComponent>(this, TEXT("InteractionSensor"));
	if (OverlapSensor)
	{
		OverlapSensor->SetupAttachment(GetOwner()->GetRootComponent());
		OverlapSensor->SetSphereRadius(SensorRadius);
		OverlapSensor->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		OverlapSensor->SetGenerateOverlapEvents(true);
		OverlapSensor->RegisterComponent();

		// Overlap delegates bind on Owner to keep dynamic binding compatible with BP.
		OverlapSensor->OnComponentBeginOverlap.AddDynamic(this, &UACDInteractionSensorComponent::HandleOnBeginOverlap);
		OverlapSensor->OnComponentEndOverlap.AddDynamic(this, &UACDInteractionSensorComponent::HandleOnEndOverlap);
	}
}

void UACDInteractionSensorComponent::HandleOnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && OtherActor->FindComponentByClass<UACDInteractableComponent>())
	{
		Candidates.Add(OtherActor);
	}
}

void UACDInteractionSensorComponent::HandleOnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Candidates.Remove(OtherActor);
	if (CurrentTarget == OtherActor)
	{
		SetCurrentTarget(nullptr);
	}
}

void UACDInteractionSensorComponent::UpdateInteractTarget()
{
	SetCurrentTarget(PickBestInteractable());
}

void UACDInteractionSensorComponent::ForceUpdate()
{
	UpdateInteractTarget();
}

bool UACDInteractionSensorComponent::TryInteract(AActor* Instigator)
{
	if (AActor* Target = CurrentTarget.Get())
	{
		if (UACDInteractableComponent* IC = Target->FindComponentByClass<UACDInteractableComponent>())
		{
			if (IC->CanInteract(Instigator))
			{
				IC->DoInteract(Instigator);

				if (IC->bSingleUse && IC->bConsumed)
				{
					SetCurrentTarget(nullptr);
				}
				return true;
			}
		}
	}
	return false;
}

// 현재는 최소 거리 Interactable actor 추출
AActor* UACDInteractionSensorComponent::PickBestInteractable() const
{
	if (Candidates.Num() == 0)
	{
		return nullptr;
	}

	AActor* BestActor = nullptr;
	const FVector OwnerLoc = GetOwner()->GetActorLocation();
	float MinDistSq = TNumericLimits<float>::Max(); // 루트 연산 전 값으로 체크
	for (const TWeakObjectPtr<AActor>& Candidate : Candidates)
	{
		if (AActor* CandidateActor = Candidate.Get())
		{
			if (!CandidateActor->FindComponentByClass<UACDInteractableComponent>())
			{
				continue;
			}

			const float CurrentDistSq = FVector::DistSquared(OwnerLoc, CandidateActor->GetActorLocation());
			if (CurrentDistSq < MinDistSq)
			{
				MinDistSq = CurrentDistSq;
				BestActor = CandidateActor;
			}
		}
	}

	return BestActor;
}

void UACDInteractionSensorComponent::SetCurrentTarget(AActor* NewTarget)
{
	if (CurrentTarget.Get() == NewTarget)
	{
		return;
	}

	CurrentTarget = NewTarget;

	FText Prompt;
	if (const AActor* Target = CurrentTarget.Get())
	{
		if (const UACDInteractableComponent* IC = Target->FindComponentByClass<UACDInteractableComponent>())
		{
			Prompt = IC->PromptText;
		}
	}

	OnTargetChanged.Broadcast(CurrentTarget.Get(), Prompt);
}
