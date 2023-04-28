// Copyright Epic Games, Inc. All Rights Reserved.

#include "LudumDareJamGameMode.h"
#include "LudumDareJamCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALudumDareJamGameMode::ALudumDareJamGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
