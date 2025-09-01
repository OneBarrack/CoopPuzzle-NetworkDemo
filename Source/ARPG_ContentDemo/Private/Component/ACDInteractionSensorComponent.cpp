// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ACDInteractionSensorComponent.h"
#include "Component/ACDInteractableComponent.h"
#include "Components/SphereComponent.h"
#include "Interface/ACDInteractionInterface.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

UACDInteractionSensorComponent::UACDInteractionSensorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
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

// 오버랩 이벤트 바인딩 및 주기적 타깃 갱신 시작
void UACDInteractionSensorComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Candidates.Empty();

	if (IsValid(GetOwner()) && GetOwner()->HasAuthority())
	{
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

void UACDInteractionSensorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(UACDInteractionSensorComponent, CurrentTargetActor, COND_OwnerOnly);
}

void UACDInteractionSensorComponent::OnRegister()
{
	Super::OnRegister();
		
	// 이미 만들어져 있으면 재생성 하지 않음
	if (SensorSphere || !GetOwner() || !GetOwner()->HasAuthority())
	{
		return;
	}

	SensorSphere = NewObject<USphereComponent>(GetOwner(), USphereComponent::StaticClass(), NAME_None, RF_Transient);
	if (SensorSphere)
	{
		// 센서 기본 설정
		SensorSphere->InitSphereRadius(SensorRadius);
		SensorSphere->SetCollisionProfileName(TEXT("InteractionSensor"));
		SensorSphere->SetGenerateOverlapEvents(true);
		SensorSphere->SetCanEverAffectNavigation(false);
		SensorSphere->SetHiddenInGame(true);

		if (USceneComponent* OwnerRoot = GetOwner()->GetRootComponent())
		{
			SensorSphere->SetupAttachment(OwnerRoot);
		}

		SensorSphere->RegisterComponent();

		SensorSphere->OnComponentBeginOverlap.AddDynamic(this, &UACDInteractionSensorComponent::HandleOnBeginOverlap);
		SensorSphere->OnComponentEndOverlap.AddDynamic(this, &UACDInteractionSensorComponent::HandleOnEndOverlap);
	}
}

void UACDInteractionSensorComponent::OnUnregister()
{
	// OnRegister에서 만들었던 런타임 컴포넌트 정리
	if (IsValid(SensorSphere))
	{
		SensorSphere->DestroyComponent();
		SensorSphere = nullptr;
	}

	Super::OnUnregister();
}

// 후보 액터가 InteractableComponent를 가지고 있다면 등록
void UACDInteractionSensorComponent::HandleOnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && OtherActor->GetClass()->ImplementsInterface(UACDInteractionInterface::StaticClass()))
	{
		Candidates.Add(OtherActor);
	}
}

// 후보 제거, 현재 타깃이 나가면 해제
void UACDInteractionSensorComponent::HandleOnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Candidates.Remove(OtherActor);
	if (GetCurrentTargetActor() == OtherActor)
	{
		SetCurrentTarget(nullptr);
	}
}

void UACDInteractionSensorComponent::OnRep_CurrentTargetActor()
{
	OnTargetChanged.Broadcast(GetCurrentTargetActor());
}

void UACDInteractionSensorComponent::UpdateInteractTarget()
{
	SetCurrentTarget(PickBestInteractable());
}

void UACDInteractionSensorComponent::ForceUpdate()
{
	UpdateInteractTarget();
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

// 타깃이 바뀔 때만 FOnInteractTargetChanged 델리게이트 브로드캐스트
void UACDInteractionSensorComponent::SetCurrentTarget(AActor* NewTargetActor)
{
	if (GetCurrentTargetActor() == NewTargetActor)
	{
		return;
	}

	CurrentTargetActor = NewTargetActor;

	const FString TargetActorName = IsValid(CurrentTargetActor) ? *CurrentTargetActor->GetName() : FString(TEXT(""));
	UE_LOG(LogTemp, Log, TEXT("[%s] Changed current target : [%s]"), ANSI_TO_TCHAR(__FUNCTION__), *TargetActorName);
}
