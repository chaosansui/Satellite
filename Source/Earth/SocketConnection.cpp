// Fill out your copyright notice in the Description page of Project Settings.


#include "SocketConnection.h"

ASocketConnection::ASocketConnection()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // 初始化服务器地址和端口
    ServerAddress = TEXT("127.0.0.5");
    ServerPort = 9999;
}

void ASocketConnection::BeginPlay()
{
    Super::BeginPlay();

    // 连接到服务器，查看是否连接成功
    if (ConnectToServer())
    {
        if (bIsConnected)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Connected to server successfully!"));
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to connect to server!"));
        }
    }
}

void ASocketConnection::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

bool ASocketConnection::ConnectToServer()
{
    //这行代码创建了一个套接字对象，并将其赋值给名为 MySocket 的指针变量。
    //ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM) 返回了当前平台的套接字子系统。
    //CreateSocket() 是套接字子系统的一个方法，用于创建一个套接字对象。
    //NAME_Stream 指定了套接字类型，表示使用流式套接字（即TCP套接字）。
    //"default" 是套接字的描述，你可以自定义它。
    //最后一个参数是一个布尔值，用于指定套接字是否是非阻塞的。在这里，它被设置为 false，表示套接字是阻塞的。
    MySocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

    // 设置服务器地址结构
    /*这行代码创建了一个用于存储服务器地址信息的 FInternetAddr 对象，并将其赋值给名为 ServerAddr 的智能指针。
        CreateInternetAddr() 是套接字子系统的一个方法，用于创建一个 FInternetAddr 对象。*/
    TSharedPtr<FInternetAddr> ServerAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
    
    
    //bIsValid 是一个布尔变量，用于指示IP地址是否有效。
    bool bIsValid;

    /*这行代码设置了服务器的IP地址。
        ServerAddress 是一个 FString 类型的变量，包含了服务器的IP地址。
        SetIp() 是 FInternetAddr 类的一个方法，用于设置IP地址。*/
    ServerAddr->SetIp(*ServerAddress, bIsValid);

    /*这行代码设置了服务器的端口号。
        ServerPort 是一个整数变量，包含了服务器的端口号。
        SetPort() 是 FInternetAddr 类的一个方法，用于设置端口号。*/
    ServerAddr->SetPort(ServerPort);

    // 连接到服务器
    //这行代码尝试连接到服务器。
    //MySocket 是一个 FSocket 类的指针，之前被创建并赋值为一个套接字对象。
    //Connect(*ServerAddr) 是 FSocket 类的一个方法，用于连接到指定的地址。
    if (MySocket && MySocket->Connect(*ServerAddr))
    {
        bIsConnected = true;
        return true;
    }
    else
    {
        bIsConnected = false;
        return false;
    }
}
