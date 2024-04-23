// Fill out your copyright notice in the Description page of Project Settings.


#include "Satellite.h"
#include "TimerManager.h" // ȷ������ TimerManager ͷ�ļ�
// Sets default values
ASatellite::ASatellite()
{
 	// �������actorÿһ֡�������Tick()���������Ҫ���Թر���������ܡ�
    PrimaryActorTick.bCanEverTick = true;

    // ����һ�����������Ϊ��
    USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
    RootComponent = RootSceneComponent;

    // ������һ����̬����������������ӵ����������
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent1"));
    StaticMeshComponent->SetupAttachment(RootComponent);

    // �����ڶ�����̬����������������ӵ����������
    StaticMeshComponent1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent2"));
    StaticMeshComponent1->SetupAttachment(RootComponent);

    // ����λ��
    SetActorLocation(FVector(6000.f, 0.f, 0.f));

    Speed = 5.f; // Ĭ���ƶ��ٶ�
    TargetIndex = 0;
}

// Called when the game starts or when spawned
void ASatellite::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASatellite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    MoveToNextTarget(DeltaTime);

}
void ASatellite::MoveToNextTarget(float deltatime)
{
    if (TargetLocations.Num() == 0)
    {
        return;
    }

    FVector Direction = (TargetLocations[TargetIndex] - GetActorLocation()).GetSafeNormal();
    FVector NewLocation = GetActorLocation() + Direction * Speed * GetWorld()->GetDeltaSeconds();

    // �ƶ�����һ��Ŀ��λ��
    SetActorLocation(TargetLocations[TargetIndex]);

    // ����Ŀ��λ������
    if (++TargetIndex >= TargetLocations.Num())
    {
        TargetIndex = 0;
    }
}


