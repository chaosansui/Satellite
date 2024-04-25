// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "IWebSocket.h"
#include "WebSocketClientGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class EARTH_API UWebSocketClientGameInstance : public UGameInstance
{
	GENERATED_BODY()


public:
	virtual void Init() override;
	virtual void Shutdown() override;

	TSharedPtr<IWebSocket> WebSocket;

	void SendJsonMessageToRegister(const FString& Mid);
	void SendJsonMessageToCreateSatellite(const FString& Mid);
	void SendJsonMessageToCreateSatelliteSpeed(const FString& Mid,const FString& type);
	void SendJsonMessageToForward(const FString& Mid,const int32& timeSlot,const FString& unit);

	void ReceiveJsonResponseFromRegister(const FString& JsonString);
	void ReceiveJsonResponseFromSatellite(const FString& JsonString);
public:
	FString uuid;
	FString sid;
};


