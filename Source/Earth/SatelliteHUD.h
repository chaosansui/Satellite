// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SatelliteHUD.generated.h"

/**
 * 
 */
UCLASS()
class EARTH_API ASatelliteHUD : public AHUD
{
	GENERATED_BODY()

	virtual void DrawHUD() override;
};
