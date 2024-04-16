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
	//成功连接
	WebSocket->OnConnected().AddLambda([]()
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,TEXT("Successfully"));
		
	});
	//连接失败
	WebSocket->OnConnectionError().AddLambda([](const FString& Error)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f,FColor::Red, TEXT("Error"));

	});
	//关闭
	WebSocket->OnClosed().AddLambda([](int32 StatusCode, const FString& Reason,bool bWasclean)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f,bWasclean ? FColor::Green : FColor::Red, FString::Printf(TEXT("Close：%s"), *Reason));
	});
	//接收信息
	WebSocket->OnMessage().AddLambda([this](const FString& Message)
	{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, FString::Printf(TEXT("On：%s"), *Message));
			ReceiveJsonResponseFromRegister(*Message);
	});
	//发送信息
	WebSocket->OnMessageSent().AddLambda([](const FString& MassageString)
	{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Send：%s"), *MassageString));

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

//注册
void UWebSocketClientGameInstance::SendJsonMessageToRegister(const FString& Mid)
{
	// 生成 requestId,基于时间戳和随机数的组合
	FGuid FirstRequestId = FGuid::NewGuid();

	// 将 requestId 转换为字符串
	FString RequestIdString = FirstRequestId.ToString(EGuidFormats::DigitsWithHyphens);
	// 构建 JSON 对象
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField("mid", Mid);
	JsonObject->SetStringField("requestId", RequestIdString);

	// 序列化 JSON 数据
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	// 发送 JSON 数据到 WebSocket 服务器
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

//解析服务器所传出的uuid并赋值给新的ServerUUID
void UWebSocketClientGameInstance::ReceiveJsonResponseFromRegister(const FString& JsonString)
{
	// 解析 JSON 数据
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		// 检查是否存在 "data" 字段
		if (JsonObject->HasField("data"))
		{
			// 获取 "data" 字段的值
			const TSharedPtr<FJsonObject>* DataObject;
			if (JsonObject->TryGetObjectField("data", DataObject))
			{
				// 提取 "uuid" 字段的值
				if ((*DataObject)->HasField("uuid"))
				{
					uuid = (*DataObject)->GetStringField("uuid");
					UE_LOG(LogTemp, Log, TEXT("Success to deserialize JSON response from server. UUID: %s"), *uuid);
				}
				else
				{
					// 如果 "data" 字段中不存在 "uuid" 字段，则记录错误日志
					UE_LOG(LogTemp, Error, TEXT("'data' field in JSON response does not contain 'uuid' field"));
				}
			}
			else
			{
				// 如果 "data" 字段的值不是对象，则记录错误日志
				UE_LOG(LogTemp, Error, TEXT("'data' field in JSON response is not an object"));
			}
		}
		else
		{
			// 如果不存在 "data" 字段，记录错误日志并抛出异常
			UE_LOG(LogTemp, Error, TEXT("JSON response from server does not contain 'data' field. This is an unexpected error."));
		}
	}
	else
	{
		// 解析失败，打印错误日志或执行其他处理
		UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON response from server"));
	}
}


void UWebSocketClientGameInstance::SendJsonMessageToCreateSatellite(const FString& Mid)
{
	// 生成 requestId,基于时间戳和随机数的组合
	FGuid FirstRequestId = FGuid::NewGuid();

	// 将 requestId 转换为字符串
	FString RequestIdString = FirstRequestId.ToString(EGuidFormats::DigitsWithHyphens);

	// 构建 JSON 对象
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField("mid", Mid);
	JsonObject->SetStringField("requestId", RequestIdString);
	JsonObject->SetStringField("'uuid'", uuid);
	UE_LOG(LogTemp, Log, TEXT("Success to deserialize JSON response from server. UUID: %s"), *uuid);
	//JsonObject->SetStringField("data",);


	// 序列化 JSON 数据
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	// 发送 JSON 数据到 WebSocket 服务器
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
