// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGridPathfinding.h"

#include "GridShapeData.h"
#include "Grid.h"
#include "MyPathFindingData.h"
#include "My_Utilities.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/Array.h"
#include "MyDebugTextAndColorsOnTiles.h"
#include "MyUnit.h"

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
	// if(IsPathFinding)
	// {
	// 	FindPathInterval();	
	// }
	
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
	// TArray<FIntPoint> result;
	// switch (MyGrid->GetGridShapeType())
	// {
	// case EGridShape::Square:
	TArray<FIntPoint> result = GetNeighborIndexesForSquare(index);
		// break;
	// case EGridShape::Hexagon:
	// 	result = GetNeighborIndexesForHexagon(index);
	// 	break;
	// case EGridShape::Triangle:
	// 	result = GetNeighborIndexesForTriangle(index);
	// }
	CurSelectedPoint = index;
	// auto pCenterData = MyGrid->GetTileDataByIndex(index);
	// float cz = pCenterData->Transform.GetLocation().Z;
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
	bool isRemove = FMathf::Abs(z - center_z) > MyGrid->GetGridTileSize().Z;
	if(isRemove)return isRemove;

	
	return (pData->TileType == ETileType::FlyingUnitsOnly && !IsFlyUnit);
}

void AMyGridPathfinding::FindPathSetting(bool isIncludeDiagonals)
{
	IncludeDiagonals = isIncludeDiagonals;
}

void AMyGridPathfinding::CanFly(bool fly)
{
	IsFlyUnit = fly;
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
		FMyPathFindingData& data = PathFindingData[index];

		if(parent.CostFromStart + data.CostToEnterTile <  data.CostFromStart)
		{
			data.CostFromStart = parent.CostFromStart + data.CostToEnterTile;
			data.PreviousIndex = parent.Index;
		}
		return MoveTemp(data);
	}
	
	auto data = AddPathFindingData(&parent,index);

	DiscoveredTileIndexes.Add(index);

	if(IsShowCost||IsShowStart||IsShowTarget)
	{
		MyDebugTextAndColorsOnTiles->ShowDebugInfo(data,IsShowCost,IsShowStart,IsShowTarget);
	}
	
	return MoveTemp(data);
	
}

FMyPathFindingData AMyGridPathfinding::AddPathFindingData(const FMyPathFindingData* ParentData, const FIntPoint& Index)
{
	FMyPathFindingData data;
	data.Index = Index;
	auto pTileData = MyGrid->GetTileDataByIndex(Index);
	switch (pTileData->TileType)
	{
	case ETileType::None:
	case ETileType::Obstacle:
	case ETileType::FlyingUnitsOnly:
		break;
	case ETileType::Normal:
		data.CostToEnterTile = 10;
		break;
	case ETileType::DoubleCost:
		data.CostToEnterTile = 20;
		break;
	case ETileType::TripleCost:
		data.CostToEnterTile = 30;
	}

	if(ParentData == nullptr)
	{
		data.CostFromStart = 0;
		data.MinimumCostToTarget = GetMinimumCostBetweenTwoTiles(Index,TargetPoint,IncludeDiagonals);
		data.PreviousIndex = FIntPoint(-999,-999);
	}
	else
	{
		//是否是斜着走
		int32 deltaX = Index.X - ParentData->Index.X;
		int32 deltaY = Index.Y - ParentData->Index.Y;
		// 判断是否为斜侧方向移动
		const bool bIsDiagonal = FMath::Abs(deltaX) == 1 && FMath::Abs(deltaY) == 1;
		data.CostFromStart = ParentData->CostFromStart + (bIsDiagonal ? data.CostToEnterTile+data.CostToEnterTile/2 : data.CostToEnterTile);//GetMinimumCostBetweenTwoTiles(Index,StartPoint,IncludeDiagonals);
		// data.CostFromStart = ParentData->CostFromStart + data.CostToEnterTile;
		data.MinimumCostToTarget = GetMinimumCostBetweenTwoTiles(Index,TargetPoint,IncludeDiagonals)*10;
		data.PreviousIndex = ParentData->Index;	
	}
	
	PathFindingData.Add(data.Index,data);
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
			// if(Diagonal)
			// {
			// 	return x > y ? x : y;
			// }
			return x + y;
		case EGridShape::Hexagon:
			return FMathf::Max((y - x)/2,0) + x;
	}
	return x+y;
}

FMyPathFindingData AMyGridPathfinding::PullCheapestTileOutOfDiscoveredList()
{
	//探しリスト中で一番コストの安いタイル引き出して
	//探しリストから削除する
	//分析リストに挿入
	//找到 探索列表中最便宜的 
	//从 探索链表中删除　
	//加入 分析队列 
	auto iter = DiscoveredTileIndexes.begin();
	FIntPoint cheapest = *iter;
	++iter;
	for(;iter != DiscoveredTileIndexes.end();++iter)
	{
		const FMyPathFindingData& a = PathFindingData[cheapest];
		const FMyPathFindingData& b = PathFindingData[*iter];
		int aval = a.CostFromStart + a.MinimumCostToTarget;
		int bval = b.CostFromStart + b.MinimumCostToTarget;
		if(bval < aval)
		{
			cheapest = b.Index;
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
	// bool isFound = false;
	// FMyPathFindingData currentTile;
	// for(int i = 0;i < MaxCalculationPerFrame;i++)
	// {
	// 	currentTile = PullCheapestTileOutOfDiscoveredList();
	// 	isFound = DiscoverTile(currentTile);
	// 	if(isFound)break;
	// }
	//
	//
	// if(!isFound && !DiscoveredTileIndexes.IsEmpty())
	// {
	// 	return;
	// }
	//
	// TArray<FIntPoint> path;
	// if(isFound)
	// {
	// 	while(currentTile.Index != FIntPoint(-999,-999))
	// 	{
	// 		path.Add(currentTile.Index);
	// 		currentTile = PathFindingData.FindRef(currentTile.PreviousIndex);
	// 	}
	// 	Algo::Reverse(path);
	// 	path.Add(TargetPoint);
	// }
	// FindPathCb.Execute(path);
	// IsPathFinding = false;
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
	
	// FMyPathFindingData startData;
	// startData.Index = start;
	// startData.CostFromStart = 0;
	// startData.CostToEnterTile = 1;
	// startData.MinimumCostToTarget = GetMinimumCostBetweenTwoTiles(start,target,IncludeDiagonals);
	// PathFindingData.Add(startData.Index,startData);
	AddPathFindingData(nullptr,start);
	
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


void AMyGridPathfinding::UnitFindPath(const int UnitSide,const FIntPoint& Start,const FIntPoint& Target,TArray<ETileType> WalkableTileTypes,FPathFindingCompleted completed,bool bDiagonal)
{
	if(IsInputDataValid(Start,Target))return;
	IncludeDiagonals = bDiagonal;
	// for(const auto& pair:PathFindingData)
	// {
	// 	MyDebugTextAndColorsOnTiles->ClearDebugInfo(pair.Value);
	// }

	DiscoveredTileIndexes.Empty();
	AnalysedTileIndexes.Empty();
	PathFindingData.Empty();
	
	StartPoint = Start;
	TargetPoint = Target;

	AddPathFindingData(nullptr,StartPoint);
	DiscoveredTileIndexes.Add(StartPoint);
	
	Async(EAsyncExecution::TaskGraphMainThread,[this,WalkableTileTypes,completed,UnitSide]()-> void
	{
		// UE_LOG(LogTemp,Log,TEXT("Start Async EAsyncExecution::TaskGraph"))
		FMyPathFindingData currentTile;
		bool isFound;
		for(;;)
		{
			currentTile = PullCheapestTileOutOfDiscoveredList();
			isFound = DiscoverTileByWalkableType(UnitSide,currentTile,WalkableTileTypes);
			if(isFound)break;

			if(!isFound && DiscoveredTileIndexes.IsEmpty())
			{
				break;
			}
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
		completed.Execute(path);
		// UE_LOG(LogTemp,Log,TEXT("Finish Async EAsyncExecution::TaskGraph"))
	});
}


bool AMyGridPathfinding::DiscoverTileByWalkableType(const int UnitSide,const FMyPathFindingData& TilePathData,const TArray<ETileType>& WalkableType)
{
	FIntPoint index = TilePathData.Index;
	TArray<FIntPoint>  neighbors = GetNeighborIndexesForSquare(index);
	//开始剔除单位无法行走的格子
	auto Predicate = [&](const FIntPoint& Element) -> bool
	{
		auto pData = MyGrid->GetTileDataByIndex(Element);
		if(pData == nullptr)return true;
		//非同一阵营的人
		if(pData->UnitOnTile != nullptr)return UnitSide != pData->UnitOnTile->GetRuntimeProperty().UnitSide;
		//是否可以行走
		if(!IsTileTypeWalkable(pData->TileType))return true;
		float z = pData->Transform.GetLocation().Z;
		//高低差
		auto pCenterData = MyGrid->GetTileDataByIndex(index);
		float center_z = pCenterData->Transform.GetLocation().Z;
		bool isRemove = FMathf::Abs(z - center_z) > MyGrid->GetGridTileSize().Z;
		if(isRemove)return isRemove;
		//是否是单位可以行走的格子类型
		bool Walkable = false;
		for(const ETileType& t : WalkableType)
		{
			if(t == pData->TileType)
			{
				Walkable = true;
				break;
			}
		}
		//是否是斜着走
		int32 deltaX = pData->Index.X - index.X;
		int32 deltaY = pData->Index.Y - index.Y;
		// 判断是否为斜侧方向移动
		const bool bIsDiagonal = FMath::Abs(deltaX) == 1 && FMath::Abs(deltaY) == 1;
		if(bIsDiagonal)
		{
			if(deltaX > 0 && deltaY > 0)
			{//右前方
				//上方和右方 如果有敌人不能走
				const FIntPoint Right(index.X,index.Y+1);
				const FIntPoint Up(index.X+1,index.Y);
				const auto RightUnit = MyGrid->GetUnitOnTile(Right);
				const auto UpUnit = MyGrid->GetUnitOnTile(Up);
				bool RightCanWalk = true;
				bool UpCanWalk = true;
				if(RightUnit != nullptr)
					RightCanWalk = RightUnit->IsSameUnitSide(UnitSide);
				if(UpUnit != nullptr)
					UpCanWalk = UpUnit->IsSameUnitSide(UnitSide);
				return !(RightCanWalk && UpCanWalk);
			}
			if(deltaX > 0 && deltaY < 0)
			{//左前方
				const FIntPoint Left(index.X,index.Y-1);
				const FIntPoint Up(index.X+1,index.Y);
				const auto LeftUnit = MyGrid->GetUnitOnTile(Left);
				const auto UpUnit = MyGrid->GetUnitOnTile(Up);
				bool LeftCanWalk = true;
				bool UpCanWalk = true;
				if(LeftUnit)LeftCanWalk = LeftUnit->IsSameUnitSide(UnitSide);
				if(UpUnit)UpCanWalk = UpUnit->IsSameUnitSide(UnitSide);
				return !(LeftCanWalk && UpCanWalk);
			}
			if(deltaX < 0 && deltaY < 0)
			{//左后方
				const FIntPoint Left(index.X,index.Y-1);
				const FIntPoint Down(index.X - 1,index.Y);
				const auto LeftUnit = MyGrid->GetUnitOnTile(Left);
				const auto DownUnit = MyGrid->GetUnitOnTile(Down);
				bool LeftCanWalk = true;
				bool DownCanWalk = true;
				if(LeftUnit)LeftCanWalk = LeftUnit->IsSameUnitSide(UnitSide);
				if(DownUnit)DownCanWalk = DownUnit->IsSameUnitSide(UnitSide);
				return !(LeftCanWalk && DownCanWalk);
			}
			if(deltaX < 0 && deltaY > 0)
			{//右后方
				const FIntPoint Right(index.X,index.Y+1);
				const FIntPoint Down(index.X-1,index.Y);
				const auto RightUnit = MyGrid->GetUnitOnTile(Right);
				const auto DownUnit = MyGrid->GetUnitOnTile(Down);
				bool RightCanWalk = true;
				bool DownCanWalk = true;
				if(RightUnit)RightCanWalk = RightUnit->IsSameUnitSide(UnitSide);
				if(DownUnit)DownCanWalk = DownUnit->IsSameUnitSide(UnitSide);
				return !(RightCanWalk && DownCanWalk);
			}
		}
		return !Walkable;
	};
	
	neighbors.RemoveAll(Predicate);
	//结束剔除

	//将周伟的格子加入分析队列
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

TArray<FIntPoint> AMyGridPathfinding::UnitWalkablePath(const FIntPoint& Start,int MaxWalkPoint,TArray<ETileType> WalkableTileTypes)
{
	IncludeDiagonals = false;
	PathFindingData.Empty();
	
	TArray<FIntPoint> ReachableTiles;
	TSet<FIntPoint> DiscoverTiles;
	FMyPathFindingData data = AddPathFindingData(nullptr,Start);
	bool next = true;
	int ReachableIndex = 0;
	for(;next;ReachableIndex++)
	{
		FIntPoint center = ReachableTiles.IsEmpty() ? Start : ReachableTiles[ReachableIndex - 1];
		if(DiscoverTiles.Contains(center))continue;
		DiscoverTiles.Add(center);
		data = PathFindingData[center];
		TArray<FIntPoint> neighbor = GetNeighborIndexesForSquare(center);

		auto Predicate = [&](const FIntPoint& Element) -> bool
		{
			auto pData = MyGrid->GetTileDataByIndex(Element);
			if(pData == nullptr)return true;
			if(!IsTileTypeWalkable(pData->TileType))return true;
			if(pData->UnitOnTile != nullptr)return true;
			
			float z = pData->Transform.GetLocation().Z;

			auto pCenterData = MyGrid->GetTileDataByIndex(center);
			float center_z = pCenterData->Transform.GetLocation().Z;
			bool isRemove = FMathf::Abs(z - center_z) > MyGrid->GetGridTileSize().Z;
			if(isRemove)return isRemove;

			bool Walkable = false;
			for(const ETileType& t : WalkableTileTypes)
			{
				if(t == pData->TileType)
				{
					Walkable = true;
					break;
				}
			}
	
			return !Walkable;
		};
		
		neighbor.RemoveAll(Predicate);
		
		for(const FIntPoint one : neighbor)
		{
			auto c = AddPathFindingData(&data,one);
			if(c.CostFromStart <= MaxWalkPoint)
			{
				ReachableTiles.Add(one);
			}
			else
			{
				next = false;
				break;
			}
		}
		
	}
	
	
	return MoveTemp(ReachableTiles);
}

TArray<FIntPoint> AMyGridPathfinding::UnitAbilityRange(const FIntPoint& Start,const FIntPoint& Range,int AllowableDeviation)
{
	PathFindingData.Empty();
	TArray<FIntPoint> ReachableTiles;
	TSet<FIntPoint> DiscoverTiles;
	FMyPathFindingData data = AddPathFindingData(nullptr,Start);
	bool next = true;
	int ReachableIndex = 0;
	ReachableTiles.Add(Start);
	for(;next;ReachableIndex++)
	{
		if(ReachableTiles.IsEmpty())break;
		
		FIntPoint center = ReachableTiles[ReachableIndex];
		if(DiscoverTiles.Contains(center))continue;
		DiscoverTiles.Add(center);
		data = PathFindingData[center];
		TArray<FIntPoint> neighbor = GetNeighborIndexesForSquare(center);

		auto Predicate = [&](const FIntPoint& Element) -> bool
		{
			auto pData = MyGrid->GetTileDataByIndex(Element);
			if(pData == nullptr)return true;
			if(!IsTileTypeWalkable(pData->TileType))return true;
			//高度差
			float z = pData->Transform.GetLocation().Z;
			auto pCenterData = MyGrid->GetTileDataByIndex(center);
			float center_z = pCenterData->Transform.GetLocation().Z;
			bool isRemove = FMathf::Abs(z - center_z) > MyGrid->GetGridTileSize().Z*AllowableDeviation;
			if(isRemove)return isRemove;

			return true;
		};
		
		neighbor.RemoveAll(Predicate);
		
		for(const FIntPoint one : neighbor)
		{
			auto c = AddPathFindingData(&data,one);
			if(c.CostFromStart >= Range.X && c.CostFromStart <= Range.Y)
			{
				ReachableTiles.Add(one);
			}
			else
			{
				next = false;
				break;
			}
		}
		
	}
	
	return MoveTemp(ReachableTiles);
}
