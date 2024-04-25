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
	SetActorLocation(FVector(0.0f, 900000.0f, 0.0f)); // ����������ĳ�ʼλ��Ϊ (0, 0, 1000)
	
	// ��ȡ������ĳ�ʼλ�ú�Ŀ��λ��
	FVector CameraLocation = GetActorLocation();
	FVector TargetLocation = FVector::ZeroVector; // Ŀ��λ��Ϊԭ��

	// ���������ָ��Ŀ��λ�õķ�������
	FVector DirectionToTarget = (TargetLocation - CameraLocation).GetSafeNormal();

	// ����������ת��Ϊ��ת
	FRotator NewRotation = DirectionToTarget.Rotation();

	// ����������ĳ���
	CameraComponent->SetWorldRotation(NewRotation);
}

// Called every frame
void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACameraPawn::ZoomCamera(float DeltaZoom)
{
	// ��������ֵ
		float ZoomValue = DeltaZoom * ZoomSpeed;

	// �����µ������λ��
	FVector NewLocation = CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector() * ZoomValue;

	// ��ȡ���������λ�õľ���
	float DistanceToNewLocation = FVector::Distance(NewLocation, RootComponent->GetComponentLocation());

	// ����Ƿ񳬳������ŷ�Χ
	if (DistanceToNewLocation < MinZoomDistance || DistanceToNewLocation > MaxZoomDistance)
	{
		// ������������ָ����Χ��
		DistanceToNewLocation = FMath::Clamp(DistanceToNewLocation, MinZoomDistance, MaxZoomDistance);

		// �������ƺ�ľ������¼����µ������λ��
		NewLocation = RootComponent->GetComponentLocation() + (NewLocation - RootComponent->GetComponentLocation()).GetSafeNormal() * DistanceToNewLocation;
	}

	// �����µ������λ��
	CameraComponent->SetWorldLocation(NewLocation);

}

void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ���������¼�
	PlayerInputComponent->BindAxis("Zoom", this, &ACameraPawn::ZoomCamera);

}

void ACameraPawn::MoveCamera(float HorizontalInput, float VerticalInput)
{
	// Your camera movement logic here
}


