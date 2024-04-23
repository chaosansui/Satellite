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
	float Speed; // �ƶ��ٶ�

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FVector> TargetLocations; // Ŀ��λ������

private:
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditAnywhere) // ������UE5�༭���б༭������
	UStaticMeshComponent* StaticMeshComponent1; // ������̬���������ָ��
	UPROPERTY(EditAnywhere)
	FVector CurrentTargetLocation; // ��ǰĿ��λ��
	int32 TargetIndex; // ��ǰĿ��λ������

	void MoveToNextTarget(float DeltaTime);
};
