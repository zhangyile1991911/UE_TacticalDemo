// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AssaultTips.generated.h"

UCLASS(Blueprintable)
class TBS20140401_API AAssaultTips : public AActor
{
	GENERATED_BODY()
	const int BezierPathNum = 15;
	TArray<FVector> BezierPath;
	int BezierIndex = 0;
public:
	// Sets default values for this actor's properties
	AAssaultTips();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void GenerateBezierPath(FVector Start,FVector Finish);

	UFUNCTION(BlueprintCallable)
	void UpdateBezierPath(float Val);

	UFUNCTION(BlueprintCallable)
	void StartBezier();
	
	UFUNCTION(BlueprintCallable)
	bool FinishBezier();

	FVector GetQuadraticCurvePoint(FVector startPoint, FVector endPoint, FVector ctrlPoint, float t)
	{
		return (1 - t) * (1 - t) * startPoint + 2 * (1 - t) * t * ctrlPoint + t * t * endPoint;
	}
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void DoAnimation(FVector StartPoint,FVector FinishPoint);

	UFUNCTION(BlueprintImplementableEvent)
	void StopAnimation();
};
