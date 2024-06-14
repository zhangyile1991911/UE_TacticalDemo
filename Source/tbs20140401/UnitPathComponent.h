// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GridShapeData.h"
#include "MyPathFindingData.h"
#include "UnitPathComponent.generated.h"

class AMyUnit;
class AGrid;

DECLARE_DELEGATE_OneParam(FUnitPathFindingCompleted,TArray<FIntPoint>);
// DECLARE_DELEGATE_OneParam(FUnitWalkRangeCompleted,TSet<FIntPoint>);
DECLARE_DELEGATE(FUnitWalkRangeCompleted);
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TBS20140401_API UUnitPathComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UUnitPathComponent();
	
protected:
	TMap<FIntPoint,FMyPathFindingData> PathFindingMap;

	TSet<FIntPoint> DiscoveredTileIndexes;
	TArray<FIntPoint> AnalysedTileIndexes;

	// TArray<FIntPoint> ReachableTiles;
	TSet<FIntPoint> ReachableMap;
	TSet<FIntPoint> AssaultRangeTiles;

	TSet<FIntPoint> TurnReachableMap;
	TSet<FIntPoint> TurnAssaultRangeTiles;
	

	FIntPoint StartIndex;
	FIntPoint TargetIndex;
	const int GridCost = 10;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	
	FMyPathFindingData AddPathFindingData(const FMyPathFindingData* ParentData, const FIntPoint& Index);
	FIntPoint PullCheapestTileOutOfDiscoveredList();
	FMyPathFindingData TryNextNeighbor(const FIntPoint& ParentIndex, const FIntPoint& index);
	bool DiscoverTileByWalkableType(const int UnitSide,const FIntPoint& CenterIndex,const TArray<ETileType>& WalkableType,bool bIsDiagonal);
	bool CanDiagonal(const FIntPoint& ParentIndex,const FIntPoint& NextIndex,int UnitSide);
	bool CheckIsBorder(const FIntPoint& Index,const TSet<FIntPoint>& DiscoveredTiles);
	void UnitAssaultRange(bool bIsTemp);
	void AddAssaultRange(const FIntPoint& Point,bool bIsTemp);
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	void UnitFindPathAsync(const FIntPoint& Target,FUnitPathFindingCompleted Completed,bool bDiagonal=true);
	void UnitWalkablePath(bool bIsTemp = false);
	void UnitWalkablePathAsync(FUnitWalkRangeCompleted Completed);
	
	const TSet<FIntPoint>& GetAssaultRangeTiles()const{return AssaultRangeTiles;}
	const TSet<FIntPoint>& GetReachableTiles()const{return ReachableMap;}
	const TSet<FIntPoint>& GetTurnAssaultRangeTiles()const{return TurnAssaultRangeTiles;}
	const TSet<FIntPoint>& GetTurnReachableTiles()const{return TurnReachableMap;}
	bool IsMoveInReachableTiles(const FIntPoint& one)const;
	UPROPERTY()
	TObjectPtr<AMyUnit> ParentPtr;
};
