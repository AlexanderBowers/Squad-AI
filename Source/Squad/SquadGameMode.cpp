// Copyright Epic Games, Inc. All Rights Reserved.

#include "SquadGameMode.h"
#include "SquadCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASquadGameMode::ASquadGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
