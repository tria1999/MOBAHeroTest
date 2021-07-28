// Copyright Epic Games, Inc. All Rights Reserved.

#include "MobaTestGameMode.h"
#include "MobaTestPlayerController.h"
#include "MobaTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMobaTestGameMode::AMobaTestGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AMobaTestPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}