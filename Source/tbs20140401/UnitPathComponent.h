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
	TSet<FIntPoint> ReachableMap;//这个是临时的移动范围
	TSet<FIntPoint> AssaultRangeTiles;//这个是临时的攻击范围

	TSet<FIntPoint> TurnReachableMap;//这个是这一回合开始前就计算好的
	TSet<FIntPoint> TurnAssaultRangeTiles;//这个也是这回合开始前就计算好的
	

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
	void UnitWalkablePath(uint32 CurrentTurnUnitUniqueId,bool bIsTemp = false);
	void UnitWalkablePathAsync(uint32 CurrentTurnUnitUniqueId,FUnitWalkRangeCompleted Completed);
	
	const TSet<FIntPoint>& GetAssaultRangeTiles()const{return AssaultRangeTiles;}
	const TSet<FIntPoint>& GetReachableTiles()const{return ReachableMap;}
	
	const TSet<FIntPoint>& GetTurnAssaultRangeTiles()const{return TurnAssaultRangeTiles;}
	const TSet<FIntPoint>& GetTurnReachableTiles()const{return TurnReachableMap;}
	bool IsMoveInReachableTiles(const FIntPoint& one)const;
	bool IsAssaultRangeTiles(const FIntPoint& one)const;

	void ResetReachableTiles();
	UPROPERTY()
	TObjectPtr<AMyUnit> ParentPtr;
};
