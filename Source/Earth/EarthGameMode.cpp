// Copyright Epic Games, Inc. All Rights Reserved.


#include "EarthGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "SatelliteHUD.h"
#include "CameraPawn.h"

AEarthGameMode::AEarthGameMode()
{
	// set default pawn class to our Blueprinted character
	HUDClass = ASatelliteHUD::StaticClass();

	// Set the default pawn class to be your custom camera pawn
	DefaultPawnClass =ACameraPawn::StaticClass();
}

void AEarthGameMode::BeginPlay()
{

}
