// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "HalfLife3GameMode.h"
#include "HalfLife3HUD.h"
#include "HalfLife3Character.h"
#include "UObject/ConstructorHelpers.h"

AHalfLife3GameMode::AHalfLife3GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AHalfLife3HUD::StaticClass();
}
