// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyGridPathfinding.generated.h"

class AGrid;

UCLASS()
class TBS20140401_API AMyGridPathfinding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyGridPathfinding();
protected:

	UPROPERTY()
	AGrid* MyGrid;
	
	bool IncludeDiagonals;
	FIntPoint CurSelectedPoint;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	TArray<FIntPoint> GetNeighborIndexesForSquare(const FIntPoint& index);
	TArray<FIntPoint> GetNeighborIndexesForTriangle(const FIntPoint& index);
	TArray<FIntPoint> GetNeighborIndexesForHexagon(const FIntPoint& index);

	bool ShouldRemovePoint(const FIntPoint& index);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	TArray<FIntPoint> GetValidTileNeighbors(const FIntPoint& index);
	void FindPathSetting(bool isIncludeDiagonals);
};


