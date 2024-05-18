// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPathFindingData.h"
#include "TileData.h"
#include "MyGridPathfinding.generated.h"


class AMyDebugTextAndColorsOnTiles;
class AGrid;
DECLARE_DELEGATE_OneParam(FPathFindingCompleted,TArray<FIntPoint>);

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

	UPROPERTY()
	AMyDebugTextAndColorsOnTiles* MyDebugTextAndColorsOnTiles;
	
	bool IncludeDiagonals=false;
	bool IsFlyUnit=false;
	FIntPoint CurSelectedPoint;
	
	TSet<FIntPoint> DiscoveredTileIndexes;
	TArray<FIntPoint> AnalysedTileIndexes;

	FMyPathFindingData CurrentDiscoveredTile;
	TMap<FIntPoint,FMyPathFindingData> PathFindingData;

	FIntPoint StartPoint;
	FIntPoint TargetPoint;

	bool IsShowCost;
	bool IsShowStart;
	bool IsShowTarget;
	
	// TArray<FIntPoint> Path;

	// FTimerHandle FindPathHandle;
	bool IsPathFinding = false;
	FPathFindingCompleted FindPathCb;
    int MaxCalculationPerFrame = 10;
protected:
	void FindPathInterval();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	TArray<FIntPoint> GetNeighborIndexesForSquare(const FIntPoint& index);
	TArray<FIntPoint> GetNeighborIndexesForTriangle(const FIntPoint& index);
	TArray<FIntPoint> GetNeighborIndexesForHexagon(const FIntPoint& index);

	bool ShouldRemovePoint(const FIntPoint& index);
	bool DiscoverTile(const FMyPathFindingData&);
	bool DiscoverTileByWalkableType(const FMyPathFindingData&,const TArray<ETileType>&);
	int GetMinimumCostBetweenTwoTiles(const FIntPoint& index1,const FIntPoint& index2,bool Diagonal);
	
	FMyPathFindingData PullCheapestTileOutOfDiscoveredList();
	bool IsInputDataValid(const FIntPoint& start, const FIntPoint& target);
	FMyPathFindingData TryNextNeighbor(const FMyPathFindingData& parent,const FIntPoint& index);
	FMyPathFindingData AddPathFindingData(const FMyPathFindingData* ParentData,const FIntPoint& Index);
	// bool DiscoverNextNeighbor();
	// bool AnalyseNextDiscoveredTile();
	// TArray<FIntPoint> GeneratePath();
	// AStar path finding
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	TArray<FIntPoint> GetValidTileNeighbors(const FIntPoint& index);
	void FindPathSetting(bool isIncludeDiagonals);
	void CanFly(bool fly);
	void FindPath(const FIntPoint& start,const FIntPoint& target,FPathFindingCompleted completed);
	
	void UnitFindPath(const FIntPoint& Start,const FIntPoint& Target,TArray<ETileType> WalkableTileTypes,FPathFindingCompleted completed);
	TArray<FIntPoint> UnitWalkablePath(const FIntPoint& Start,int MaxWalkPoint,TArray<ETileType> WalkableTileTypes);
	void SetShowDebugOption(bool ShowCost,bool ShowStart,bool ShowTarget)
	{
		IsShowCost = ShowCost;
		IsShowStart = ShowStart;
		IsShowTarget = ShowTarget;
	}
	void SetMaxCalculationPerFrame(int count)
	{
		count = FMathf::Clamp(count,10,500);
		UE_LOG(LogTemp,Log,TEXT("current max calculation per frame %d"),count);
		MaxCalculationPerFrame = count;
	}
	
};


