// Fill out your copyright notice in the Description page of Project Settings.


#include "SocketConnection.h"

ASocketConnection::ASocketConnection()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // ��ʼ����������ַ�Ͷ˿�
    ServerAddress = TEXT("127.0.0.5");
    ServerPort = 9999;
}

void ASocketConnection::BeginPlay()
{
    Super::BeginPlay();

    // ���ӵ����������鿴�Ƿ����ӳɹ�
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
    //���д��봴����һ���׽��ֶ��󣬲����丳ֵ����Ϊ MySocket ��ָ�������
    //ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM) �����˵�ǰƽ̨���׽�����ϵͳ��
    //CreateSocket() ���׽�����ϵͳ��һ�����������ڴ���һ���׽��ֶ���
    //NAME_Stream ָ�����׽������ͣ���ʾʹ����ʽ�׽��֣���TCP�׽��֣���
    //"default" ���׽��ֵ�������������Զ�������
    //���һ��������һ������ֵ������ָ���׽����Ƿ��Ƿ������ġ��������������Ϊ false����ʾ�׽����������ġ�
    MySocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

    // ���÷�������ַ�ṹ
    /*���д��봴����һ�����ڴ洢��������ַ��Ϣ�� FInternetAddr ���󣬲����丳ֵ����Ϊ ServerAddr ������ָ�롣
        CreateInternetAddr() ���׽�����ϵͳ��һ�����������ڴ���һ�� FInternetAddr ����*/
    TSharedPtr<FInternetAddr> ServerAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
    
    
    //bIsValid ��һ����������������ָʾIP��ַ�Ƿ���Ч��
    bool bIsValid;

    /*���д��������˷�������IP��ַ��
        ServerAddress ��һ�� FString ���͵ı����������˷�������IP��ַ��
        SetIp() �� FInternetAddr ���һ����������������IP��ַ��*/
    ServerAddr->SetIp(*ServerAddress, bIsValid);

    /*���д��������˷������Ķ˿ںš�
        ServerPort ��һ�����������������˷������Ķ˿ںš�
        SetPort() �� FInternetAddr ���һ���������������ö˿ںš�*/
    ServerAddr->SetPort(ServerPort);

    // ���ӵ�������
    //���д��볢�����ӵ���������
    //MySocket ��һ�� FSocket ���ָ�룬֮ǰ����������ֵΪһ���׽��ֶ���
    //Connect(*ServerAddr) �� FSocket ���һ���������������ӵ�ָ���ĵ�ַ��
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
