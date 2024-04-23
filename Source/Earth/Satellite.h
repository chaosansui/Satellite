// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Satellite.generated.h"

UCLASS(Blueprintable)
class EARTH_API ASatellite : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASatellite();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ExampleProperty; // Example property that can be edited in blueprint

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Speed; // 移动速度

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FVector> TargetLocations; // 目标位置数组

private:
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditAnywhere) // 允许在UE5编辑器中编辑的属性
	UStaticMeshComponent* StaticMeshComponent1; // 声明静态网格体组件指针
	UPROPERTY(EditAnywhere)
	FVector CurrentTargetLocation; // 当前目标位置
	int32 TargetIndex; // 当前目标位置索引

	void MoveToNextTarget(float DeltaTime);
};
