// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPathFindingData.h"
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
	
	TArray<FIntPoint> DiscoveredTileIndexes;
	TArray<FIntPoint> AnalysedTileIndexes;

	FMyPathFindingData CurrentDiscoveredTile;
	TMap<FIntPoint,FMyPathFindingData> PathFindingData;

	FIntPoint StartPoint;
	FIntPoint TargetPoint;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	TArray<FIntPoint> GetNeighborIndexesForSquare(const FIntPoint& index);
	TArray<FIntPoint> GetNeighborIndexesForTriangle(const FIntPoint& index);
	TArray<FIntPoint> GetNeighborIndexesForHexagon(const FIntPoint& index);

	bool ShouldRemovePoint(const FIntPoint& index);
	bool DiscoverTile(const FMyPathFindingData&);
	int GetMinimumCostBetweenTwoTiles(const FIntPoint& index1,const FIntPoint& index2,bool Diagonal);
	
	FMyPathFindingData PullCheapestTileOutOfDiscoveredList();
	bool IsInputDataValid(const FIntPoint& start, const FIntPoint& target);
	FMyPathFindingData TryNextNeighbor(const FMyPathFindingData& parent,const FIntPoint& index);
	
	// bool DiscoverNextNeighbor();
	// bool AnalyseNextDiscoveredTile();
	// TArray<FIntPoint> GeneratePath();
	//AStar path finding
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	TArray<FIntPoint> GetValidTileNeighbors(const FIntPoint& index);
	void FindPathSetting(bool isIncludeDiagonals);
	TArray<FIntPoint> FindPath(const FIntPoint& start,const FIntPoint& target);
};


