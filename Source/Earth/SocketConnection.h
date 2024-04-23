// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TcpSocketConnection.h"
#include "Networking.h" // 包含Socket库头文件
#include "SocketConnection.generated.h"

/**
 * 
 */
UCLASS()
class EARTH_API ASocketConnection : public ATcpSocketConnection
{
	GENERATED_BODY()
public:
    // Sets default values for this actor's properties
    ASocketConnection();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

private:
    FSocket* MySocket;
    FString ServerAddress;
    int32 ServerPort;

    bool ConnectToServer();
    bool bIsConnected;
};
