// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPawn.h"
#include "Satellite.h"
#include <Kismet\GameplayStatics.h>
// Sets default values
ACameraPawn::ACameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a root scene component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create camera component and attach it to the root component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(FVector(0.0f, 900000.0f, 0.0f)); // 设置摄像机的初始位置为 (0, 0, 1000)
	
	// 获取摄像机的初始位置和目标位置
	FVector CameraLocation = GetActorLocation();
	FVector TargetLocation = FVector::ZeroVector; // 目标位置为原点

	// 计算摄像机指向目标位置的方向向量
	FVector DirectionToTarget = (TargetLocation - CameraLocation).GetSafeNormal();

	// 将方向向量转换为旋转
	FRotator NewRotation = DirectionToTarget.Rotation();

	// 设置摄像机的朝向
	CameraComponent->SetWorldRotation(NewRotation);
}

// Called every frame
void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACameraPawn::ZoomCamera(float DeltaZoom)
{
	// 计算缩放值
		float ZoomValue = DeltaZoom * ZoomSpeed;

	// 计算新的摄像机位置
	FVector NewLocation = CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector() * ZoomValue;

	// 获取摄像机到新位置的距离
	float DistanceToNewLocation = FVector::Distance(NewLocation, RootComponent->GetComponentLocation());

	// 检查是否超出了缩放范围
	if (DistanceToNewLocation < MinZoomDistance || DistanceToNewLocation > MaxZoomDistance)
	{
		// 将距离限制在指定范围内
		DistanceToNewLocation = FMath::Clamp(DistanceToNewLocation, MinZoomDistance, MaxZoomDistance);

		// 根据限制后的距离重新计算新的摄像机位置
		NewLocation = RootComponent->GetComponentLocation() + (NewLocation - RootComponent->GetComponentLocation()).GetSafeNormal() * DistanceToNewLocation;
	}

	// 设置新的摄像机位置
	CameraComponent->SetWorldLocation(NewLocation);

}

void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 绑定鼠标滚轮事件
	PlayerInputComponent->BindAxis("Zoom", this, &ACameraPawn::ZoomCamera);

}

void ACameraPawn::MoveCamera(float HorizontalInput, float VerticalInput)
{
	// Your camera movement logic here
}


