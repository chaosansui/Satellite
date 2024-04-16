// Fill out your copyright notice in the Description page of Project Settings.


#include "WebSocketClientGameInstance.h"
#include "WebSocketsModule.h"
#include "Json.h"


void UWebSocketClientGameInstance::Init()
{
	Super::Init();

	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		FModuleManager::Get().LoadModule("WebSockets");
	}

	WebSocket = FWebSocketsModule::Get().CreateWebSocket("ws://127.0.0.5:8000/ws/conn");
	//�ɹ�����
	WebSocket->OnConnected().AddLambda([]()
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,TEXT("Successfully"));
		
	});
	//����ʧ��
	WebSocket->OnConnectionError().AddLambda([](const FString& Error)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f,FColor::Red, TEXT("Error"));

	});
	//�ر�
	WebSocket->OnClosed().AddLambda([](int32 StatusCode, const FString& Reason,bool bWasclean)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f,bWasclean ? FColor::Green : FColor::Red, FString::Printf(TEXT("Close��%s"), *Reason));
	});
	//������Ϣ
	WebSocket->OnMessage().AddLambda([this](const FString& Message)
	{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, FString::Printf(TEXT("On��%s"), *Message));
			ReceiveJsonResponseFromRegister(*Message);
	});
	//������Ϣ
	WebSocket->OnMessageSent().AddLambda([](const FString& MassageString)
	{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Send��%s"), *MassageString));

	});

	WebSocket->Connect();
	SendJsonMessageToRegister("register");

	SendJsonMessageToCreateSatellite("createSatellite");
	SendJsonMessageToForward("forward");
}

void UWebSocketClientGameInstance::Shutdown()
{
	if (WebSocket->IsConnected())
	{
		WebSocket->Close();
	}
	Super::Shutdown();
}

//ע��
void UWebSocketClientGameInstance::SendJsonMessageToRegister(const FString& Mid)
{
	// ���� requestId,����ʱ���������������
	FGuid FirstRequestId = FGuid::NewGuid();

	// �� requestId ת��Ϊ�ַ���
	FString RequestIdString = FirstRequestId.ToString(EGuidFormats::DigitsWithHyphens);
	// ���� JSON ����
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField("mid", Mid);
	JsonObject->SetStringField("requestId", RequestIdString);

	// ���л� JSON ����
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	// ���� JSON ���ݵ� WebSocket ������
	if (WebSocket.IsValid())
	{
		WebSocket->Send(JsonString);
		UE_LOG(LogTemp, Log, TEXT("WebSocket is connected!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WebSocket is not valid or not connected!"));
	}
};

//������������������uuid����ֵ���µ�ServerUUID
void UWebSocketClientGameInstance::ReceiveJsonResponseFromRegister(const FString& JsonString)
{
	// ���� JSON ����
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		// ����Ƿ���� "data" �ֶ�
		if (JsonObject->HasField("data"))
		{
			// ��ȡ "data" �ֶε�ֵ
			const TSharedPtr<FJsonObject>* DataObject;
			if (JsonObject->TryGetObjectField("data", DataObject))
			{
				// ��ȡ "uuid" �ֶε�ֵ
				if ((*DataObject)->HasField("uuid"))
				{
					uuid = (*DataObject)->GetStringField("uuid");
					UE_LOG(LogTemp, Log, TEXT("Success to deserialize JSON response from server. UUID: %s"), *uuid);
				}
				else
				{
					// ��� "data" �ֶ��в����� "uuid" �ֶΣ����¼������־
					UE_LOG(LogTemp, Error, TEXT("'data' field in JSON response does not contain 'uuid' field"));
				}
			}
			else
			{
				// ��� "data" �ֶε�ֵ���Ƕ������¼������־
				UE_LOG(LogTemp, Error, TEXT("'data' field in JSON response is not an object"));
			}
		}
		else
		{
			// ��������� "data" �ֶΣ���¼������־���׳��쳣
			UE_LOG(LogTemp, Error, TEXT("JSON response from server does not contain 'data' field. This is an unexpected error."));
		}
	}
	else
	{
		// ����ʧ�ܣ���ӡ������־��ִ����������
		UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON response from server"));
	}
}


void UWebSocketClientGameInstance::SendJsonMessageToCreateSatellite(const FString& Mid)
{
	// ���� requestId,����ʱ���������������
	FGuid FirstRequestId = FGuid::NewGuid();

	// �� requestId ת��Ϊ�ַ���
	FString RequestIdString = FirstRequestId.ToString(EGuidFormats::DigitsWithHyphens);

	// ���� JSON ����
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField("mid", Mid);
	JsonObject->SetStringField("requestId", RequestIdString);
	JsonObject->SetStringField("'uuid'", uuid);
	UE_LOG(LogTemp, Log, TEXT("Success to deserialize JSON response from server. UUID: %s"), *uuid);
	//JsonObject->SetStringField("data",);


	// ���л� JSON ����
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	// ���� JSON ���ݵ� WebSocket ������
	if (WebSocket.IsValid())
	{
		WebSocket->Send(JsonString);
		UE_LOG(LogTemp, Log, TEXT("Satellite is connected!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Satellite is not valid or not connected!"));
	}
}

void UWebSocketClientGameInstance::SendJsonMessageToForward(const FString& Mid)
{

}
