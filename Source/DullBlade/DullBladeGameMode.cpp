// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "DullBladeGameMode.h"
#include "DullBladePlayerController.h"
#include "DullBladeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADullBladeGameMode::ADullBladeGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ADullBladePlayerController::StaticClass();

	
}