// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ACDInteractionSensorComponent.h"
#include "Component/ACDInteractableComponent.h"
#include "Components/SphereComponent.h"
#include "Interface/ACDInteractionInterface.h"
#include "TimerManager.h"

UACDInteractionSensorComponent::UACDInteractionSensorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SensorSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSensor"));
	if (SensorSphere)
	{
		SensorSphere->SetCollisionProfileName(TEXT("InteractionSensor"));
		SensorSphere->SetGenerateOverlapEvents(true);
		SensorSphere->InitSphereRadius(SensorRadius);
	}
}

#if WITH_EDITOR
void UACDInteractionSensorComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.MemberProperty == nullptr)
		return;

	const FName PropName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropName == GET_MEMBER_NAME_CHECKED(UACDInteractionSensorComponent, SensorRadius))
	{
		if (IsValid(SensorSphere))
		{
			SensorSphere->SetSphereRadius(FMath::Max(10.f, SensorRadius), /*bUpdateOverlaps=*/true);
		}
	}
	else if (PropName == GET_MEMBER_NAME_CHECKED(UACDInteractionSensorComponent, UpdatePeriod))
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(UpdateHandle);
			if (UpdatePeriod > 0.f)
			{
				World->GetTimerManager().SetTimer(
					UpdateHandle, this, &UACDInteractionSensorComponent::UpdateInteractTarget,
					UpdatePeriod, true
				);
			}
		}
	}
}
#endif

void UACDInteractionSensorComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Candidates.Empty();

	if (IsValid(GetOwner()))
	{
		SensorSphere->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		SensorSphere->OnComponentBeginOverlap.AddDynamic(this, &UACDInteractionSensorComponent::HandleOnBeginOverlap);
		SensorSphere->OnComponentEndOverlap.AddDynamic(this, &UACDInteractionSensorComponent::HandleOnEndOverlap);

		if (UpdatePeriod > 0.f)
		{
			if (UWorld* World = GetWorld())
			{
				World->GetTimerManager().SetTimer(
				UpdateHandle, this, &UACDInteractionSensorComponent::UpdateInteractTarget,
				UpdatePeriod, true);
			}
		}
	}
}

void UACDInteractionSensorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(UpdateHandle);
	}
}

void UACDInteractionSensorComponent::HandleOnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && OtherActor->GetClass()->ImplementsInterface(UACDInteractionInterface::StaticClass()))
	{
		Candidates.Add(OtherActor);
	}
}

void UACDInteractionSensorComponent::HandleOnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Candidates.Remove(OtherActor);
	if (GetCurrentTargetActor() == OtherActor)
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
	if (AActor* TargetActor = GetCurrentTargetActor())
	{
		if (IACDInteractionInterface::Execute_CanInteract(TargetActor, GetOwner()))
		{
			IACDInteractionInterface::Execute_DoInteract(TargetActor, GetOwner());
			UpdateInteractTarget();
			
			return true;
		}
	}

	return false;
}

/*
* 최적 타깃 선택. 
* 현재는 최소 거리 Interactable actor 추출.
* 필요시 이 함수만 LineTrace 등 추가 로직 구현 해주면 된다.
*/ 
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
			if (!CandidateActor->GetClass()->ImplementsInterface(UACDInteractionInterface::StaticClass()))
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

void UACDInteractionSensorComponent::SetCurrentTarget(AActor* NewTargetActor)
{
	if (GetCurrentTargetActor() == NewTargetActor)
	{
		return;
	}

	CurrentTargetActor = NewTargetActor;

	const FString TargetActorName = IsValid(CurrentTargetActor.Get()) ? *CurrentTargetActor->GetName() : FString(TEXT(""));
	UE_LOG(LogTemp, Log, TEXT("[%s] Changed current target : [%s]"), ANSI_TO_TCHAR(__FUNCTION__), *TargetActorName);

	FText Prompt;
	if (const AActor* Target = GetCurrentTargetActor())
	{
		if (const UACDInteractableComponent* IC = Target->FindComponentByClass<UACDInteractableComponent>())
		{
			Prompt = IC->PromptText;
		}
	}
	
	OnTargetChanged.Broadcast(GetCurrentTargetActor(), Prompt);
}
