// Copyright Epic Games, Inc. All Rights Reserved.


#include "EarthGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "SatelliteHUD.h"

AEarthGameMode::AEarthGameMode()
{
	// set default pawn class to our Blueprinted character
	HUDClass = ASatelliteHUD::StaticClass();
}

void AEarthGameMode::BeginPlay()
{

}
