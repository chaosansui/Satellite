// Fill out your copyright notice in the Description page of Project Settings.


#include "Satellite.h"
#include "TimerManager.h" // 确保包含 TimerManager 头文件
// Sets default values
ASatellite::ASatellite()
{
 	// 设置这个actor每一帧都会调用Tick()。如果不需要可以关闭以提高性能。
    PrimaryActorTick.bCanEverTick = true;

    // 创建一个父级组件作为根
    USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
    RootComponent = RootSceneComponent;

    // 创建第一个静态网格体组件，并附加到父级组件上
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent1"));
    StaticMeshComponent->SetupAttachment(RootComponent);

    // 创建第二个静态网格体组件，并附加到父级组件上
    StaticMeshComponent1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent2"));
    StaticMeshComponent1->SetupAttachment(RootComponent);

    // 设置位置
    SetActorLocation(FVector(6000.f, 0.f, 0.f));

    Speed = 5.f; // 默认移动速度
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

    // 移动到下一个目标位置
    SetActorLocation(TargetLocations[TargetIndex]);

    // 更新目标位置索引
    if (++TargetIndex >= TargetLocations.Num())
    {
        TargetIndex = 0;
    }
}


