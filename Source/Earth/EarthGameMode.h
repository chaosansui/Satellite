// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EarthGameMode.generated.h"

UCLASS(minimalapi)
class AEarthGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AEarthGameMode();

protected:
	virtual void BeginPlay() override;


};



