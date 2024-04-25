// Fill out your copyright notice in the Description page of Project Settings.


#include "WebSocketClientGameInstance.h"
#include "WebSocketsModule.h"
#include "Json.h"

#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"


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
			
			ReceiveJsonResponseFromSatellite(*Message);
	});
	//������Ϣ
	/*WebSocket->OnMessageSent().AddLambda([](const FString& MassageString)
	{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Send��%s"), *MassageString));
	});*/

	WebSocket->Connect();
	SendJsonMessageToRegister("register");
	
	
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

					//�ж�uuid�Ƿ�Ϊ�գ�����Ϊ�ս�uuidֱ�ӷ��͵���ʼ������
					if (uuid.IsEmpty())
					{
						UE_LOG(LogTemp, Error, TEXT("UUID is NULL"));
					}
					else {
						SendJsonMessageToCreateSatellite("createSatellite");
						//SendJsonMessageToCreateSatelliteSpeed("createSatellite","rv");
					}
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
			//UE_LOG(LogTemp, Error, TEXT("JSON response from server does not contain 'data' field. This is an unexpected error."));
		}
	}
	else
	{
		// ����ʧ�ܣ���ӡ������־��ִ����������
		UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON response from server"));
	}
}

//ʹ��GCRS��ʼ������
void UWebSocketClientGameInstance::SendJsonMessageToCreateSatellite(const FString& Mid)
{
	// ���� requestId,����ʱ���������������
	FGuid FirstRequestId = FGuid::NewGuid();

	// �� requestId ת��Ϊ�ַ���
	FString RequestIdString = FirstRequestId.ToString(EGuidFormats::DigitsWithHyphens);

	// ����һ��������
	TSharedPtr<FJsonObject> mainObject = MakeShareable(new FJsonObject());

	// ���� "data" �ֶε�ֵ
	TSharedPtr<FJsonObject> dataObject = MakeShareable(new FJsonObject());

	// ���� "type" �ֶ�Ϊ "GCRS"
	dataObject->SetStringField("type", "GCRS");

	// ���������Ӷ���
	TSharedPtr<FJsonObject> coordinateObject = MakeShareable(new FJsonObject());
	coordinateObject->SetNumberField("x", 6000.00); // ���� "x" �ֶ�
	coordinateObject->SetNumberField("y", 6500.00); // ���� "y" �ֶ�
	coordinateObject->SetNumberField("deg", 51.61); // ���� "deg" �ֶ�

	// �������Ӷ�����ӵ� "data" �������е� "coordinate" �ֶ���
	dataObject->SetObjectField("coordinate", coordinateObject);

	// �� "data" �ֶ���ӵ���������
	mainObject->SetObjectField("data", dataObject);

	// ���������ֶΣ�mid��requestId �� uuid��
	mainObject->SetStringField("mid", Mid);
	mainObject->SetStringField("requestId", RequestIdString);
	mainObject->SetStringField("uuid", uuid);

	// ���л� JSON ����
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(mainObject.ToSharedRef(), Writer);

	// ���� JSON ���ݵ� WebSocket ������
	if (WebSocket.IsValid())
	{
		WebSocket->Send(JsonString);
		UE_LOG(LogTemp, Log, TEXT("Satellite is connected!"));
		UE_LOG(LogTemp, Log, TEXT("Success to deserialize JSON response from server. UUID: %s"), *JsonString);
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Satellite is not valid or not connected!"));
	}
}



//ʹ���ٶȳ�ʼ��һ������
//void UWebSocketClientGameInstance::SendJsonMessageToCreateSatelliteSpeed(const FString& Mid,const FString& type)
//{
//	// ���� requestId,����ʱ���������������
//	FGuid FirstRequestId = FGuid::NewGuid();
//
//	// �� requestId ת��Ϊ�ַ���
//	FString RequestIdString = FirstRequestId.ToString(EGuidFormats::DigitsWithHyphens);
//
//	// ����һ��������
//	TSharedPtr<FJsonObject> mainObject = MakeShareable(new FJsonObject());
//
//	// ���� "data" �ֶε�ֵ
//	TSharedPtr<FJsonObject> dataObject = MakeShareable(new FJsonObject());
//
//	// ���� "type" �ֶ�Ϊ "rv"
//	dataObject->SetStringField("type", type);
//
//	// ���������Ӷ���
//	TSharedPtr<FJsonObject> coordinateObject = MakeShareable(new FJsonObject());
//	// ���� "r" �� "v" �ֶ�
//	TArray<double> r = { -2384.46, 5729.01, 3050.46 };
//	TArray<double> v = { -7.3618, -2.98997, -2.98997 };
//
//	// �� double ����ת���� JsonValue ����
//	TArray<TSharedPtr<FJsonValue>> rJsonValues;
//	for (double value : r) {
//		rJsonValues.Add(MakeShareable(new FJsonValueNumber(value)));
//	}
//
//	TArray<TSharedPtr<FJsonValue>> vJsonValues;
//	for (double value : v) {
//		vJsonValues.Add(MakeShareable(new FJsonValueNumber(value)));
//	}
//
//	// �� JsonValue �������õ� coordinateObject ��
//	coordinateObject->SetArrayField(TEXT("r"), rJsonValues);
//	coordinateObject->SetArrayField(TEXT("v"), vJsonValues);
//
//	// �������Ӷ�����ӵ� "data" �������е� "coordinate" �ֶ���
//	dataObject->SetObjectField("coordinate", coordinateObject);
//
//	// �� "data" �ֶ���ӵ���������
//	mainObject->SetObjectField("data", dataObject);
//
//	// ���������ֶΣ�mid��requestId �� uuid��
//	mainObject->SetStringField("mid", Mid);
//	mainObject->SetStringField("requestId", RequestIdString);
//	mainObject->SetStringField("uuid", uuid);
//
//	// ���л� JSON ����
//	FString JsonString;
//	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
//	FJsonSerializer::Serialize(mainObject.ToSharedRef(), Writer);
//
//	// ���� JSON ���ݵ� WebSocket ������
//	if (WebSocket.IsValid())
//	{
//		WebSocket->Send(JsonString);
//
//	}
//	else
//	{
//		UE_LOG(LogTemp, Error, TEXT("Satellite is not valid or not connected!"));
//	}
//}



//������ʼ���������
void UWebSocketClientGameInstance::ReceiveJsonResponseFromSatellite(const FString& JsonString)
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
				// ��ȡ "sid" �ֶε�ֵ
				if ((*DataObject)->HasField("sid"))
				{
					sid = (*DataObject)->GetStringField("sid");
					//�ж�sid�Ƿ�Ϊ�գ�����Ϊ�ս�sidֱ�ӷ��͵���ʼ������
					if (sid.IsEmpty())
					{
						UE_LOG(LogTemp, Error, TEXT("sid is NULL"));
					}
					else 
					{
						SendJsonMessageToForward("forward",5,"minute");
						//SendJsonMessageToCreateSatelliteSpeed("createSatellite");
					}
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
			//UE_LOG(LogTemp, Error, TEXT("JSON response from server does not contain 'data' field. This is an unexpected error."));
		}
	}
	else
	{
		// ����ʧ�ܣ���ӡ������־��ִ����������
		UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON response from server"));
	}
}

//��һ������
void UWebSocketClientGameInstance::SendJsonMessageToForward(const FString& Mid,const int32& timeSlot,const FString& unit)
{
	// ���� requestId,����ʱ���������������
	FGuid FirstRequestId = FGuid::NewGuid();

	// �� requestId ת��Ϊ�ַ���
	FString RequestIdString = FirstRequestId.ToString(EGuidFormats::DigitsWithHyphens);

	// ����һ��������
	TSharedPtr<FJsonObject> mainObject = MakeShareable(new FJsonObject());

	// ���� "data" �ֶε�ֵ
	TSharedPtr<FJsonObject> dataObject = MakeShareable(new FJsonObject());


	 //���������Ӷ���
	TSharedPtr<FJsonObject> coordinateObject = MakeShareable(new FJsonObject());
	// ���� "r" �� "v" �ֶ�
	TArray<double> r = { -2384.46, 5729.01, 3050.46 };
	TArray<double> v = { -7.3618, -2.98997, -2.98997 };

	// �� double ����ת���� JsonValue ����
	TArray<TSharedPtr<FJsonValue>> rJsonValues;
	for (double value : r) {
		rJsonValues.Add(MakeShareable(new FJsonValueNumber(value)));
	}

	TArray<TSharedPtr<FJsonValue>> vJsonValues;
	for (double value : v) {
		vJsonValues.Add(MakeShareable(new FJsonValueNumber(value)));
	}

	// �� JsonValue �������õ� coordinateObject ��
	coordinateObject->SetArrayField(TEXT("r"), rJsonValues);
	coordinateObject->SetArrayField(TEXT("v"), vJsonValues);
	dataObject->SetObjectField("coordinate", coordinateObject);
	dataObject->SetStringField("sid", sid);
	dataObject->SetNumberField("timeSlot", timeSlot);
	dataObject->SetStringField("unit", unit);

	// �� "data" �ֶ���ӵ���������
	mainObject->SetObjectField("data", dataObject);

	// ���������ֶΣ�mid��requestId �� uuid��
	mainObject->SetStringField("mid", Mid);
	mainObject->SetStringField("requestId", RequestIdString);
	mainObject->SetStringField("uuid", uuid);
	// ���л� JSON ����
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(mainObject.ToSharedRef(), Writer);

	// ���� JSON ���ݵ� WebSocket ������
	if (WebSocket.IsValid())
	{
		WebSocket->Send(JsonString);
		UE_LOG(LogTemp, Log, TEXT("forword is connected!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Forward is not valid or not connected!"));
	}
}
