// Copyright Epic Games, Inc. All Rights Reserved.

#include "ARPG_ContetDemoGameMode.h"
#include "ARPG_ContetDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AARPG_ContetDemoGameMode::AARPG_ContetDemoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
