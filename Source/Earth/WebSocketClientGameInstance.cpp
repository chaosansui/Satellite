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
			
			ReceiveJsonResponseFromSatellite(*Message);
	});
	//发送信息
	/*WebSocket->OnMessageSent().AddLambda([](const FString& MassageString)
	{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Send：%s"), *MassageString));
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

					//判断uuid是否为空，若不为空将uuid直接发送到初始化卫星
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
			//UE_LOG(LogTemp, Error, TEXT("JSON response from server does not contain 'data' field. This is an unexpected error."));
		}
	}
	else
	{
		// 解析失败，打印错误日志或执行其他处理
		UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON response from server"));
	}
}

//使用GCRS初始化卫星
void UWebSocketClientGameInstance::SendJsonMessageToCreateSatellite(const FString& Mid)
{
	// 生成 requestId,基于时间戳和随机数的组合
	FGuid FirstRequestId = FGuid::NewGuid();

	// 将 requestId 转换为字符串
	FString RequestIdString = FirstRequestId.ToString(EGuidFormats::DigitsWithHyphens);

	// 创建一个主对象
	TSharedPtr<FJsonObject> mainObject = MakeShareable(new FJsonObject());

	// 创建 "data" 字段的值
	TSharedPtr<FJsonObject> dataObject = MakeShareable(new FJsonObject());

	// 设置 "type" 字段为 "GCRS"
	dataObject->SetStringField("type", "GCRS");

	// 创建坐标子对象
	TSharedPtr<FJsonObject> coordinateObject = MakeShareable(new FJsonObject());
	coordinateObject->SetNumberField("x", 6000.00); // 设置 "x" 字段
	coordinateObject->SetNumberField("y", 6500.00); // 设置 "y" 字段
	coordinateObject->SetNumberField("deg", 51.61); // 设置 "deg" 字段

	// 将坐标子对象添加到 "data" 主对象中的 "coordinate" 字段下
	dataObject->SetObjectField("coordinate", coordinateObject);

	// 将 "data" 字段添加到主对象中
	mainObject->SetObjectField("data", dataObject);

	// 设置其他字段（mid、requestId 和 uuid）
	mainObject->SetStringField("mid", Mid);
	mainObject->SetStringField("requestId", RequestIdString);
	mainObject->SetStringField("uuid", uuid);

	// 序列化 JSON 数据
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(mainObject.ToSharedRef(), Writer);

	// 发送 JSON 数据到 WebSocket 服务器
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



//使用速度初始化一个卫星
//void UWebSocketClientGameInstance::SendJsonMessageToCreateSatelliteSpeed(const FString& Mid,const FString& type)
//{
//	// 生成 requestId,基于时间戳和随机数的组合
//	FGuid FirstRequestId = FGuid::NewGuid();
//
//	// 将 requestId 转换为字符串
//	FString RequestIdString = FirstRequestId.ToString(EGuidFormats::DigitsWithHyphens);
//
//	// 创建一个主对象
//	TSharedPtr<FJsonObject> mainObject = MakeShareable(new FJsonObject());
//
//	// 创建 "data" 字段的值
//	TSharedPtr<FJsonObject> dataObject = MakeShareable(new FJsonObject());
//
//	// 设置 "type" 字段为 "rv"
//	dataObject->SetStringField("type", type);
//
//	// 创建坐标子对象
//	TSharedPtr<FJsonObject> coordinateObject = MakeShareable(new FJsonObject());
//	// 设置 "r" 和 "v" 字段
//	TArray<double> r = { -2384.46, 5729.01, 3050.46 };
//	TArray<double> v = { -7.3618, -2.98997, -2.98997 };
//
//	// 将 double 数组转换成 JsonValue 数组
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
//	// 将 JsonValue 数组设置到 coordinateObject 中
//	coordinateObject->SetArrayField(TEXT("r"), rJsonValues);
//	coordinateObject->SetArrayField(TEXT("v"), vJsonValues);
//
//	// 将坐标子对象添加到 "data" 主对象中的 "coordinate" 字段下
//	dataObject->SetObjectField("coordinate", coordinateObject);
//
//	// 将 "data" 字段添加到主对象中
//	mainObject->SetObjectField("data", dataObject);
//
//	// 设置其他字段（mid、requestId 和 uuid）
//	mainObject->SetStringField("mid", Mid);
//	mainObject->SetStringField("requestId", RequestIdString);
//	mainObject->SetStringField("uuid", uuid);
//
//	// 序列化 JSON 数据
//	FString JsonString;
//	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
//	FJsonSerializer::Serialize(mainObject.ToSharedRef(), Writer);
//
//	// 发送 JSON 数据到 WebSocket 服务器
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



//解析初始化后的卫星
void UWebSocketClientGameInstance::ReceiveJsonResponseFromSatellite(const FString& JsonString)
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
				// 提取 "sid" 字段的值
				if ((*DataObject)->HasField("sid"))
				{
					sid = (*DataObject)->GetStringField("sid");
					//判断sid是否为空，若不为空将sid直接发送到初始化卫星
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
			//UE_LOG(LogTemp, Error, TEXT("JSON response from server does not contain 'data' field. This is an unexpected error."));
		}
	}
	else
	{
		// 解析失败，打印错误日志或执行其他处理
		UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON response from server"));
	}
}

//进一步卫星
void UWebSocketClientGameInstance::SendJsonMessageToForward(const FString& Mid,const int32& timeSlot,const FString& unit)
{
	// 生成 requestId,基于时间戳和随机数的组合
	FGuid FirstRequestId = FGuid::NewGuid();

	// 将 requestId 转换为字符串
	FString RequestIdString = FirstRequestId.ToString(EGuidFormats::DigitsWithHyphens);

	// 创建一个主对象
	TSharedPtr<FJsonObject> mainObject = MakeShareable(new FJsonObject());

	// 创建 "data" 字段的值
	TSharedPtr<FJsonObject> dataObject = MakeShareable(new FJsonObject());


	 //创建坐标子对象
	TSharedPtr<FJsonObject> coordinateObject = MakeShareable(new FJsonObject());
	// 设置 "r" 和 "v" 字段
	TArray<double> r = { -2384.46, 5729.01, 3050.46 };
	TArray<double> v = { -7.3618, -2.98997, -2.98997 };

	// 将 double 数组转换成 JsonValue 数组
	TArray<TSharedPtr<FJsonValue>> rJsonValues;
	for (double value : r) {
		rJsonValues.Add(MakeShareable(new FJsonValueNumber(value)));
	}

	TArray<TSharedPtr<FJsonValue>> vJsonValues;
	for (double value : v) {
		vJsonValues.Add(MakeShareable(new FJsonValueNumber(value)));
	}

	// 将 JsonValue 数组设置到 coordinateObject 中
	coordinateObject->SetArrayField(TEXT("r"), rJsonValues);
	coordinateObject->SetArrayField(TEXT("v"), vJsonValues);
	dataObject->SetObjectField("coordinate", coordinateObject);
	dataObject->SetStringField("sid", sid);
	dataObject->SetNumberField("timeSlot", timeSlot);
	dataObject->SetStringField("unit", unit);

	// 将 "data" 字段添加到主对象中
	mainObject->SetObjectField("data", dataObject);

	// 设置其他字段（mid、requestId 和 uuid）
	mainObject->SetStringField("mid", Mid);
	mainObject->SetStringField("requestId", RequestIdString);
	mainObject->SetStringField("uuid", uuid);
	// 序列化 JSON 数据
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(mainObject.ToSharedRef(), Writer);

	// 发送 JSON 数据到 WebSocket 服务器
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
