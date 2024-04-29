// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGridPathfinding.h"

#include "GridShapeData.h"
#include "Grid.h"
#include "MyPathFindingData.h"
#include "My_Utilities.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/Array.h"
#include "MyDebugTextAndColorsOnTiles.h"

// Sets default values
AMyGridPathfinding::AMyGridPathfinding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyGridPathfinding::BeginPlay()
{
	Super::BeginPlay();
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(),AGrid::StaticClass());
	MyGrid = Cast<AGrid>(actor);

	actor = UGameplayStatics::GetActorOfClass(GetWorld(),AMyDebugTextAndColorsOnTiles::StaticClass());
	MyDebugTextAndColorsOnTiles = Cast<AMyDebugTextAndColorsOnTiles>(actor);
}

// Called every frame
void AMyGridPathfinding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsPathFinding)
	{
		FindPathInterval();	
	}
	
}

TArray<FIntPoint> AMyGridPathfinding::GetNeighborIndexesForSquare(const FIntPoint& index)
{
	TArray<FIntPoint> result;
	result.Add(FIntPoint(index.X+1,index.Y));
	result.Add(FIntPoint(index.X,index.Y+1));
	result.Add(FIntPoint(index.X-1,index.Y));
	result.Add(FIntPoint(index.X,index.Y-1));
	if(!IncludeDiagonals)return result;

	result.Add(FIntPoint(index.X+1,index.Y+1));
	result.Add(FIntPoint(index.X-1,index.Y+1));
	result.Add(FIntPoint(index.X-1,index.Y-1));
	result.Add(FIntPoint(index.X+1,index.Y-1));
	return result;
}

TArray<FIntPoint> AMyGridPathfinding::GetNeighborIndexesForTriangle(const FIntPoint& index)
{
	TArray<FIntPoint> result;
	result.Add(FIntPoint(index.X,index.Y-1));
	result.Add(FIntPoint(index.X,index.Y+1));

	if(IsIntEven(index.X) == IsIntEven(index.Y))
	{
		result.Add(FIntPoint(index.X,index.Y));
		result.Add(FIntPoint(index.X-1,index.Y+2));
		result.Add(FIntPoint(index.X-1,index.Y-2));
	}
	else
	{
		result.Add(FIntPoint(index.X-1,index.Y));
		result.Add(FIntPoint(index.X+1,index.Y+2));
		result.Add(FIntPoint(index.X+1,index.Y-2));
	}

	// if(!IncludeDiagonals)return result;
	
	return result;
}

TArray<FIntPoint> AMyGridPathfinding::GetNeighborIndexesForHexagon(const FIntPoint& index)
{
	TArray<FIntPoint> result;
	//left
	result.Add(FIntPoint(index.X,index.Y-2));
	//right
	result.Add(FIntPoint(index.X,index.Y+2));
	//top
	result.Add(FIntPoint(index.X+1,index.Y-1));
	result.Add(FIntPoint(index.X+1,index.Y+1));
	//bottom
	result.Add(FIntPoint(index.X-1,index.Y-1));
	result.Add(FIntPoint(index.X-1,index.Y+1));
	return result;
}

TArray<FIntPoint> AMyGridPathfinding::GetValidTileNeighbors(const FIntPoint& index)
{
	TArray<FIntPoint> result;
	switch (MyGrid->GetGridShapeType())
	{
	case EGridShape::Square:
		result = GetNeighborIndexesForSquare(index);
		break;
	case EGridShape::Hexagon:
		result = GetNeighborIndexesForHexagon(index);
		break;
	case EGridShape::Triangle:
		result = GetNeighborIndexesForTriangle(index);
	}
	CurSelectedPoint = index;
	auto pCenterData = MyGrid->GetTileDataByIndex(index);
	float cz = pCenterData->Transform.GetLocation().Z;
	result.RemoveAll([this](const FIntPoint& one) { return ShouldRemovePoint(one); });
	
	return MoveTemp(result);
}

bool AMyGridPathfinding::ShouldRemovePoint(const FIntPoint& index)
{
	auto pData = MyGrid->GetTileDataByIndex(index);
	if(pData == nullptr)return true;
	if(!IsTileTypeWalkable(pData->TileType))return true;
	float z = pData->Transform.GetLocation().Z;

	auto pCenterData = MyGrid->GetTileDataByIndex(CurSelectedPoint);
	float center_z = pCenterData->Transform.GetLocation().Z;
	return FMathf::Abs(z - center_z) > MyGrid->GetGridTileSize().Z;
}

void AMyGridPathfinding::FindPathSetting(bool isIncludeDiagonals)
{
	IncludeDiagonals = isIncludeDiagonals;
}

//AStar path finding
bool AMyGridPathfinding::IsInputDataValid(const FIntPoint& start, const FIntPoint& target)
{
	if(start == target)return false;

	if(MyGrid->IsTileWalkable(start))return false;

	if(MyGrid->IsTileWalkable(target))return false;

	return true;
}

FMyPathFindingData AMyGridPathfinding::TryNextNeighbor(const FMyPathFindingData& parent, const FIntPoint& index)
{
	if(DiscoveredTileIndexes.Contains(index))
	{
		auto indexPoint = DiscoveredTileIndexes.FindByKey(index);
		FMyPathFindingData data = PathFindingData.FindRef(*indexPoint);
		// int cost = GetMinimumCostBetweenTwoTiles(parent.Index,index,IncludeDiagonals);
		if(parent.CostFromStart + data.CostToEnterTile <  data.CostFromStart)
		{
			data.CostFromStart = parent.CostFromStart + data.CostToEnterTile;
			data.PreviousIndex = parent.Index;
		}
		return MoveTemp(data);
	}

	FMyPathFindingData data;
	data.Index = index;
	data.CostFromStart = GetMinimumCostBetweenTwoTiles(index,StartPoint,IncludeDiagonals);
	// data.CostToEnterTile = GetMinimumCostBetweenTwoTiles(index,parent.Index,IncludeDiagonals);
	data.MinimumCostToTarget = GetMinimumCostBetweenTwoTiles(index,TargetPoint,IncludeDiagonals);
	data.PreviousIndex = parent.Index;
	PathFindingData.Add(data.Index,data);
	DiscoveredTileIndexes.Add(index);

	if(IsShowCost||IsShowStart||IsShowTarget)
	{
		MyDebugTextAndColorsOnTiles->ShowDebugInfo(data,IsShowCost,IsShowStart,IsShowTarget);
	}
	
	return MoveTemp(data);
	
}

int AMyGridPathfinding::GetMinimumCostBetweenTwoTiles(const FIntPoint& index1,const FIntPoint& index2,bool Diagonal)
{
	FIntPoint result = index1 - index2;
	float x = FMathf::Abs(result.X);
	float y = FMathf::Abs(result.Y);
	switch (MyGrid->GetGridShapeType())
	{
		case EGridShape::Triangle:
		case EGridShape::Square:
			if(Diagonal)
			{
				return x > y ? x : y;
			}
		
			return x + y;
		case EGridShape::Hexagon:
			return FMathf::Max((y - x)/2,0) + x;
	}
	return x+y;
}

FMyPathFindingData AMyGridPathfinding::PullCheapestTileOutOfDiscoveredList()
{
	//找到 探索列表中最便宜的 探しリスト中で一番安いタイル引き出して
	//从 探索链表中删除　探しリストから削除する
	//加入 分析队列 分析リストに挿入
	FIntPoint cheapest = DiscoveredTileIndexes[0];
	for(FIntPoint one : DiscoveredTileIndexes)
	{
		const FMyPathFindingData& ct = PathFindingData[cheapest];
		const FMyPathFindingData& ot = PathFindingData[one];
		if(ot.CostFromStart + ot.CostToEnterTile + ot.MinimumCostToTarget < ct.CostFromStart + ct.CostToEnterTile+ct.MinimumCostToTarget)
		{
			cheapest = ot.Index;
			break;
		}
	}
	DiscoveredTileIndexes.Remove(cheapest);
	AnalysedTileIndexes.Add(cheapest);
	return PathFindingData[cheapest];
}


bool AMyGridPathfinding::DiscoverTile(const FMyPathFindingData& TilePathData)
{
	TArray<FIntPoint>  neighbors = GetValidTileNeighbors(TilePathData.Index);
	
	for(const FIntPoint& one : neighbors)
	{
		if(!AnalysedTileIndexes.Contains(one))
		{
			FMyPathFindingData nextPoint = TryNextNeighbor(TilePathData,one);
			if(nextPoint.Index == TargetPoint)return true;
		}
	}
	return false;
}

void AMyGridPathfinding::FindPathInterval()
{
	FMyPathFindingData currentTile = PullCheapestTileOutOfDiscoveredList();
	bool isFound = DiscoverTile(currentTile);

	if(!isFound && !DiscoveredTileIndexes.IsEmpty())
	{
		return;
	}

	TArray<FIntPoint> path;
	if(isFound)
	{
		while(currentTile.Index != FIntPoint(-999,-999))
		{
			path.Add(currentTile.Index);
			currentTile = PathFindingData.FindRef(currentTile.PreviousIndex);
		}
		Algo::Reverse(path);
		path.Add(TargetPoint);
	}
	FindPathCb.Execute(path);
	IsPathFinding = false;
	// GetWorld()->GetTimerManager().ClearTimer(FindPathHandle);
}

void AMyGridPathfinding::FindPath(const FIntPoint& start, const FIntPoint& target,FPathFindingCompleted completed)
{
	// Path.Empty();
	if(IsInputDataValid(start,target))return;

	// GetWorld()->GetTimerManager().SetTimer(FindPathHandle, this, &AMyGridPathfinding::FindPathInterval, 5.0f, true);

	for(const auto& pair:PathFindingData)
	{
		MyDebugTextAndColorsOnTiles->ClearDebugInfo(pair.Value);
	}
	
	DiscoveredTileIndexes.Empty();
	AnalysedTileIndexes.Empty();
	PathFindingData.Empty();
	
	StartPoint = start;
	TargetPoint = target;
	
	FMyPathFindingData startData;
	startData.Index = start;
	startData.CostFromStart = 0;
	startData.CostToEnterTile = 1;
	startData.MinimumCostToTarget = GetMinimumCostBetweenTwoTiles(start,target,IncludeDiagonals);
	PathFindingData.Add(startData.Index,startData);
	
	DiscoveredTileIndexes.Add(start);

	IsPathFinding = true;
	FindPathCb = completed;

	// GetWorld()->GetTimerManager().SetTimer(FindPathHandle, this, &AMyGridPathfinding::FindPathInterval, 1.0f, true);

	
	// bool isFound = false;
	// FMyPathFindingData currentTile;
	// while(!isFound && !DiscoveredTileIndexes.IsEmpty())
	// {
	// 	currentTile = PullCheapestTileOutOfDiscoveredList();
	// 	isFound = DiscoverTile(currentTile);
	// }
	// if(!isFound)
	// {
	// 	return path;
	// }
	//
	// while(currentTile.Index != FIntPoint(-999,-999))
	// {
	// 	path.Add(currentTile.Index);
	// 	currentTile = PathFindingData.FindRef(currentTile.PreviousIndex);
	// }
	// Algo::Reverse(path);
	// path.Add(target);
	//
	//
	// return path;
}
