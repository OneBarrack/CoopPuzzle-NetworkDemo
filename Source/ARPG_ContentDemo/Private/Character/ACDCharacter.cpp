// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ACDCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Component/ACDInteractionSensorComponent.h"
#include "Interface/ACDInteractionInterface.h"

AACDCharacter::AACDCharacter()
{
	InteractionSensor = CreateDefaultSubobject<UACDInteractionSensorComponent>(TEXT("InteractionSenSor"));
    InteractionSensor->SetIsReplicated(true);
}

void AACDCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AACDCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (auto* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (InteractAction)
        {
            EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &AACDCharacter::Interact);
        }
    }
}

void AACDCharacter::Interact()
{
    if (!IsValid(InteractionSensor)) return;

    // 클라: 현 타깃만 읽고 서버에 요청
    if (IsLocallyControlled())
    {
        if (AActor* TargetActor = InteractionSensor->GetCurrentTargetActor())
        {
            Server_Interact(TargetActor);
        }
    }
}

void AACDCharacter::Server_Interact_Implementation(AActor* TargetActor)
{
    if (!IsValid(TargetActor) || !IsValid(InteractionSensor)) return;

    // 서버에서 최종 검증
    if (IACDInteractionInterface::Execute_CanInteract(TargetActor, this))
    {
        // 서버가 실제 상호작용 실행 (컴포넌트/액터 내부 로직 호출은 서버 권한에서)
        IACDInteractionInterface::Execute_DoInteract(TargetActor, this);
        InteractionSensor->ForceUpdate();
    }
}
