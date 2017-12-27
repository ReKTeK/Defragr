// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "DefragrGameMode.h"
#include "Defragr.h"
#include "DefragrHUD.h"
#include "DefragrPlayer.h"

ADefragrGameMode::ADefragrGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Defragr/Blueprints/DefragrPlayerInstance"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ADefragrHUD::StaticClass();
}
