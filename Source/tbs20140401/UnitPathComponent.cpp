// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitPathComponent.h"

#include "Grid.h"

#include "MyUnit.h"
#include "My_Utilities.h"
#include "Async/Async.h"

using namespace UE::Tasks;

// Sets default values for this component's properties
UUnitPathComponent::UUnitPathComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UUnitPathComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	// ReachableTiles.Reserve(64);
	// BorderTileIndexes.Reserve(64);
	AssaultRangeTiles.Reserve(64);
}


bool IsInputDataValid(const FIntPoint& start, const FIntPoint& target,AGrid* MyGrid)
{
	if(start == target)return false;

	if(MyGrid->IsTileWalkable(start))return false;

	if(MyGrid->IsTileWalkable(target))return false;

	return true;
}

int GetMinimumCostBetweenTwoTiles(const FIntPoint& index1, const FIntPoint& index2)
{
	FIntPoint result = index1 - index2;
	float x = FMathf::Abs(result.X);
	float y = FMathf::Abs(result.Y);
	return x + y;
}

FIntPoint UUnitPathComponent::PullCheapestTileOutOfDiscoveredList()
{
	auto iter = DiscoveredTileIndexes.begin();
	FIntPoint cheapest = *iter;
	++iter;
	for(;iter != DiscoveredTileIndexes.end();++iter)
	{
		const FMyPathFindingData& a = PathFindingMap[cheapest];
		const FMyPathFindingData& b = PathFindingMap[*iter];
		int aval = a.CostFromStart + a.MinimumCostToTarget;
		int bval = b.CostFromStart + b.MinimumCostToTarget;
		if(bval < aval)
		{
			cheapest = b.Index;
		}
	}
	DiscoveredTileIndexes.Remove(cheapest);
	AnalysedTileIndexes.Add(cheapest);
	return cheapest;
}

FMyPathFindingData UUnitPathComponent::TryNextNeighbor(const FIntPoint& ParentIndex, const FIntPoint& index)
{
	const auto& ParentData = PathFindingMap[ParentIndex];
	if(DiscoveredTileIndexes.Contains(index))
	{
		FMyPathFindingData& data = PathFindingMap[index];

		if(ParentData.CostFromStart + data.CostToEnterTile <  data.CostFromStart)
		{
			data.CostFromStart = ParentData.CostFromStart + data.CostToEnterTile;
			data.PreviousIndex = ParentData.Index;
		}
		return MoveTemp(data);
	}
	
	auto data = AddPathFindingData(&ParentData,index);

	DiscoveredTileIndexes.Add(index);
	
	return MoveTemp(data);
	
}


FMyPathFindingData UUnitPathComponent::AddPathFindingData(const FMyPathFindingData* ParentData, const FIntPoint& Index)
{
	FMyPathFindingData data;
	data.Index = Index;
	const auto TileData = ParentPtr->MyGrid->GetTileDataByIndex(Index);
	switch (TileData->TileType)
	{
	case ETileType::None:
	case ETileType::Obstacle:
	case ETileType::FlyingUnitsOnly:
		break;
	case ETileType::Normal:
		data.CostToEnterTile = GridCost;
		break;
	case ETileType::DoubleCost:
		data.CostToEnterTile = GridCost * 2;
		break;
	case ETileType::TripleCost:
		data.CostToEnterTile = GridCost * 3;
	}

	if(ParentData == nullptr)
	{
		data.CostFromStart = 0;
		data.MinimumCostToTarget = GetMinimumCostBetweenTwoTiles(Index,TargetIndex);
		data.PreviousIndex = FIntPoint(-999,-999);
	}
	else
	{
		//是否是斜着走
		int32 deltaX = Index.X - ParentData->Index.X;
		int32 deltaY = Index.Y - ParentData->Index.Y;
		// 判断是否为斜侧方向移动
		const bool bIsDiagonal = FMath::Abs(deltaX) == 1 && FMath::Abs(deltaY) == 1;
		data.CostFromStart = ParentData->CostFromStart + (bIsDiagonal ? data.CostToEnterTile+data.CostToEnterTile/2 : data.CostToEnterTile);
		data.MinimumCostToTarget = GetMinimumCostBetweenTwoTiles(Index,TargetIndex)*GridCost;
		data.PreviousIndex = ParentData->Index;	
	}
	
	PathFindingMap.Add(data.Index,data);
	return MoveTemp(data);
}

bool UUnitPathComponent::CanDiagonal(const FIntPoint& ParentIndex,const FIntPoint& NextIndex,int UnitSide)
{
	//是否是斜着走
	int32 deltaX = NextIndex.X - ParentIndex.X;
	int32 deltaY = NextIndex.Y - ParentIndex.Y;
	
	// 判断是否为斜侧方向移动
	const bool bMoveDiagonal = FMath::Abs(deltaX) == 1 && FMath::Abs(deltaY) == 1;
	if(bMoveDiagonal)
	{
		if(deltaX > 0 && deltaY > 0)
		{//右前方
			//上方和右方 如果有敌人不能走
			const FIntPoint Right(ParentIndex.X,ParentIndex.Y+1);
			const FIntPoint Up(ParentIndex.X+1,ParentIndex.Y);
			const auto RightUnit = ParentPtr->MyGrid->GetUnitOnTile(Right);
			const auto UpUnit = ParentPtr->MyGrid->GetUnitOnTile(Up);
			bool RightCanWalk = true;
			bool UpCanWalk = true;

			if(RightUnit != nullptr)
				RightCanWalk = RightUnit->IsFriend(UnitSide);
			else
				RightCanWalk = false;
			
			if(UpUnit != nullptr)
				UpCanWalk = UpUnit->IsFriend(UnitSide);
			else
				UpCanWalk = false;
			
			if(RightCanWalk && UpCanWalk)return true;
		}
		else if(deltaX > 0 && deltaY < 0)
		{//左前方
			const FIntPoint Left(ParentIndex.X,ParentIndex.Y-1);
			const FIntPoint Up(ParentIndex.X+1,ParentIndex.Y);
			const auto LeftUnit = ParentPtr->MyGrid->GetUnitOnTile(Left);
			const auto UpUnit = ParentPtr->MyGrid->GetUnitOnTile(Up);
			bool LeftCanWalk = true;
			bool UpCanWalk = true;

			if(LeftUnit)LeftCanWalk = LeftUnit->IsFriend(UnitSide);
			else LeftCanWalk = false;

			if(UpUnit)UpCanWalk = UpUnit->IsFriend(UnitSide);
			else UpCanWalk = false;
			
			if(LeftCanWalk && UpCanWalk)return true;
		}
		else if(deltaX < 0 && deltaY < 0)
		{//左后方
			const FIntPoint Left(ParentIndex.X,ParentIndex.Y-1);
			const FIntPoint Down(ParentIndex.X - 1,ParentIndex.Y);
			const auto LeftUnit = ParentPtr->MyGrid->GetUnitOnTile(Left);
			const auto DownUnit = ParentPtr->MyGrid->GetUnitOnTile(Down);
			bool LeftCanWalk = true;
			bool DownCanWalk = true;
			if(LeftUnit)LeftCanWalk = LeftUnit->IsFriend(UnitSide);
			else LeftCanWalk = false;
			
			if(DownUnit)DownCanWalk = DownUnit->IsFriend(UnitSide);
			else DownCanWalk = false;
			
			if(LeftCanWalk && DownCanWalk)return true;
		}
		else if(deltaX < 0 && deltaY > 0)
		{//右后方
			const FIntPoint Right(ParentIndex.X,ParentIndex.Y+1);
			const FIntPoint Down(ParentIndex.X-1,ParentIndex.Y);
			const auto RightUnit = ParentPtr->MyGrid->GetUnitOnTile(Right);
			const auto DownUnit = ParentPtr->MyGrid->GetUnitOnTile(Down);
			bool RightCanWalk = true;
			bool DownCanWalk = true;
			if(RightUnit)RightCanWalk = RightUnit->IsFriend(UnitSide);
			else RightCanWalk = false;
			
			if(DownUnit)DownCanWalk = DownUnit->IsFriend(UnitSide);
			else DownCanWalk = false;
			if(RightCanWalk && DownCanWalk)return true;
		}
	}
	return false;
}

const TArray<FIntPoint> Neighbors = {FIntPoint(1,0),FIntPoint(0,1),FIntPoint(-1,0),FIntPoint(0,-1),
FIntPoint(1,1),FIntPoint(-1,1),FIntPoint(-1,-1),FIntPoint(1,-1)};
bool UUnitPathComponent::DiscoverTileByWalkableType(const int UnitSide,const FIntPoint& CenterIndex,const TArray<ETileType>& WalkableType,bool bIsDiagonal)
{
	int Count = bIsDiagonal ? 8 : 4;
	for(int i = 0;i < Count;i++)
	{
		FIntPoint NextIndex = CenterIndex + Neighbors[i];

		if(AnalysedTileIndexes.Contains(NextIndex))continue;

		const auto CenterTileDataPtr = ParentPtr->MyGrid->GetTileDataByIndex(CenterIndex);
		const auto NextTileDataPtr = ParentPtr->MyGrid->GetTileDataByIndex(NextIndex);
		if(NextTileDataPtr == nullptr)continue;
		if(NextTileDataPtr->UnitOnTile != nullptr && !NextTileDataPtr->UnitOnTile->IsFriend(UnitSide))
		{
			continue;
		}
		if(!IsTileTypeWalkable(NextTileDataPtr->TileType))continue;
		const float z = NextTileDataPtr->Transform.GetLocation().Z;
		//高低差
		// const float center_z = NextTileDataPtr->Transform.GetLocation().Z;
		// const bool isRemove = FMathf::Abs(z - center_z) > ParentPtr->MyGrid->GetGridTileSize().Z;
		const bool isRemove = NextTileDataPtr->Height - CenterTileDataPtr->Height > ParentPtr->GetHeightTolerance();
		if(isRemove)continue;
		
		if(!WalkableType.Contains(NextTileDataPtr->TileType))continue;;
		
		//是否是斜着走
		const int32 deltaX = NextIndex.X - CenterIndex.X;
		const int32 deltaY = NextIndex.Y - CenterIndex.Y;
		
		// 判断是否为斜侧方向移动
		const bool bMoveDiagonal = FMath::Abs(deltaX) == 1 && FMath::Abs(deltaY) == 1;
		if(bMoveDiagonal)
		{
			if(!CanDiagonal(CenterIndex,NextIndex,ParentPtr->GetUnitSide()))
				continue;
		}
		
		//将周伟的格子加入分析队列
		if(!AnalysedTileIndexes.Contains(NextIndex))
		{
			FMyPathFindingData nextPoint = TryNextNeighbor(CenterIndex,NextIndex);
			if(nextPoint.Index == TargetIndex)return true;
		}
	}
	
	return false;
}


// Called every frame
void UUnitPathComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}




void UUnitPathComponent::UnitFindPathAsync(const FIntPoint& Target, FUnitPathFindingCompleted Completed,bool bDiagonal)
{
	if(IsInputDataValid(ParentPtr->GridIndex,Target,ParentPtr->MyGrid))return;
	
	StartIndex = ParentPtr->GridIndex;
	TargetIndex = Target;
	
	
	Async(EAsyncExecution::TaskGraphMainThread,[this,Completed,bDiagonal]()
	{
		PathFindingMap.Empty();
		DiscoveredTileIndexes.Empty();
		AnalysedTileIndexes.Empty();
		
		AddPathFindingData(nullptr,StartIndex);
		DiscoveredTileIndexes.Add(StartIndex);
		FIntPoint CurrentTile;
		bool isFound;
		for(;;)
		{
			CurrentTile = PullCheapestTileOutOfDiscoveredList();
			isFound = DiscoverTileByWalkableType(ParentPtr->GetUnitSide(),CurrentTile,ParentPtr->MyStats.ValidTileTypes,bDiagonal);
			if(isFound)break;
	
			if(!isFound && DiscoveredTileIndexes.IsEmpty())
			{
				break;
			}
		}
	
		FMyPathFindingData* Iter = &PathFindingMap[CurrentTile];
		TArray<FIntPoint> path;
		path.Reserve(16);
		if(isFound)
		{
			while(Iter != nullptr && Iter->Index != FIntPoint(-999,-999))
			{
				path.Add(Iter->Index);
				if(PathFindingMap.Contains(Iter->PreviousIndex))
				{
					Iter = &PathFindingMap[Iter->PreviousIndex];	
				}
				else
				{
					Iter = nullptr;
				}
			}
			Algo::Reverse(path);
			path.Add(TargetIndex);
		}
		Completed.Execute(MoveTemp(path));
	});
}

bool UUnitPathComponent::CheckIsBorder(const FIntPoint& Index,const TSet<FIntPoint>& DiscoveredTiles)
{
	bool bIsBorder = false;
	for(int i = 0;i < 4;i++)
	{//检查周围格子 是否 都被加入探索队列
		FIntPoint Element = Index + Neighbors[i];
		
		if(DiscoveredTiles.Contains(Element))continue;
		//周围有无效格子 说明是边界
		const auto TileDataPtr = ParentPtr->MyGrid->GetTileDataByIndex(Element);
		if(TileDataPtr == nullptr)
		{
			bIsBorder = true;
			break;
		}
		
		//周围有障碍
		if(TileDataPtr->TileType == ETileType::Obstacle)
		{
			bIsBorder = true;
			break;
		}
		
		//是否已经在探索格子里了
		if(!DiscoveredTiles.Contains(Element))
		{
			bIsBorder = true;
			break;
		}
	}
	return bIsBorder;
}

void UUnitPathComponent::UnitWalkablePath(uint32 CurrentTurnUnitUniqueId,bool bIsTemp)
{
	PathFindingMap.Empty();

	FIntPoint Start = ParentPtr->GridIndex;
	TArray<FIntPoint> ReachableTiles;
	
	DiscoveredTileIndexes.Empty();
	ReachableTiles.Add(Start);
	DiscoveredTileIndexes.Add(Start);
	AddPathFindingData(nullptr,Start);
	bool next = true;
	int ReachableIndex = 0;
	for(;next;ReachableIndex++)
	{
		if(ReachableTiles.IsEmpty() || ReachableIndex >= ReachableTiles.Num())break;
		
		const FIntPoint& center = ReachableTiles[ReachableIndex];
		const FMyPathFindingData& CenterDataRef = PathFindingMap[center];
		for(int i = 0;i < 4;i++)
		{//广度优先
			FIntPoint Element = center + Neighbors[i];
			const auto TileDataPtr = ParentPtr->MyGrid->GetTileDataByIndex(Element);
			if(TileDataPtr == nullptr)continue;
			if(!IsTileTypeWalkable(TileDataPtr->TileType))continue;
			if(TileDataPtr->States.Contains(ETileState::Selected))continue;
			if(TileDataPtr->UnitOnTile != nullptr)
			{
				if(TileDataPtr->UnitOnTile->IsFriend(ParentPtr->GetUnitSide())==false)
				{//是敌人
					if(TileDataPtr->UnitOnTile->GetUniqueID() != CurrentTurnUnitUniqueId)
					{//但不是当前行动回合的敌人，只能跳过
						continue;
					}	
				}
			}
				
			//高低差
			float z = TileDataPtr->Transform.GetLocation().Z;
			const auto CenterTileDataPtr = ParentPtr->MyGrid->GetTileDataByIndex(center);
			float cz = CenterTileDataPtr->Transform.GetLocation().Z;
			const bool bOverHeight = FMathf::Abs(z - cz) > ParentPtr->MyGrid->GetGridTileSize().Z;
			if(bOverHeight)continue;;
			
			//是否是单位可以行走的格子类型
			bool bCanPass = ParentPtr->MyStats.ValidTileTypes.Contains(TileDataPtr->TileType);
			if(!bCanPass)continue;
			//添加格子
			if(!DiscoveredTileIndexes.Contains(Element))
			{
				auto c = AddPathFindingData(&CenterDataRef,Element);
				if(c.CostFromStart <= ParentPtr->GetMove()*GridCost)
				{
					// UE_LOG(LogTemp,Log,TEXT("添加格子 X = %d Y = %d"),center.X,center.Y);
					ReachableTiles.Add(Element);
					DiscoveredTileIndexes.Add(Element);
				}
				else
				{
					PathFindingMap.Remove(c.Index);
					next = false;
					break;
				}
			}
		}
	}

	if(bIsTemp)
	{
		ReachableMap.Empty();
		for(const auto& one: ReachableTiles)
		{
			ReachableMap.Add(one);
		}
	}
	else
	{
		TurnReachableMap.Empty();
		for(const auto& one: ReachableTiles)
		{
			TurnReachableMap.Add(one);
		}
		ReachableMap = TurnReachableMap;
	}
	
	
	UnitAssaultRange(bIsTemp);
}

void UUnitPathComponent::UnitWalkablePathAsync(uint32 CurrentTurnUnitUniqueId,FUnitWalkRangeCompleted Completed)
{
	Async(EAsyncExecution::TaskGraphMainThread,[this,CurrentTurnUnitUniqueId,Completed]()
	{
		UnitWalkablePath(CurrentTurnUnitUniqueId,true);
		if(Completed.IsBound())
		{
			// TSet<FIntPoint> Result;
			// Result.Append(ReachableMap);
			// Result.Append(AssaultRangeTiles);
			Completed.Execute();
		}
	});
}

bool UUnitPathComponent::IsMoveInReachableTiles(const FIntPoint& one) const
{
	if(ReachableMap.Contains(one))
		return true;
	if(TurnReachableMap.Contains(one))
		return true;
	return false;
}

bool UUnitPathComponent::IsAssaultRangeTiles(const FIntPoint& one) const
{
	if(AssaultRangeTiles.Contains(one))
		return true;
	if(TurnAssaultRangeTiles.Contains(one))
		return true;
	return false;
}

void UUnitPathComponent::AddAssaultRange(const FIntPoint& Point,bool bIsTemp)
{
	// UE_LOG(LogTemp,Log,TEXT("AddAssaultRange %d %d"),Point.X,Point.Y)
	if(bIsTemp)
	{
		if(AssaultRangeTiles.Contains(Point))return;
		if(!ParentPtr->MyGrid->IsValidGridIndex(Point))return;
		if(ReachableMap.Contains(Point))return;
		AssaultRangeTiles.Add(Point);	
	}
	else
	{
		if(TurnAssaultRangeTiles.Contains(Point))return;
		if(!ParentPtr->MyGrid->IsValidGridIndex(Point))return;
		if(TurnReachableMap.Contains(Point))return;
		TurnAssaultRangeTiles.Add(Point);	
	}
	
}

void UUnitPathComponent::UnitAssaultRange(bool bIsTemp)
{
	//检查是否为边界
	TSet<FIntPoint> BorderTileIndexes;
	for(auto& Pair : PathFindingMap)
	{
		if(CheckIsBorder(Pair.Key,ReachableMap))
		{
			// UE_LOG(LogTemp,Log,TEXT(" x = %d y = %d is border"),Pair.Key.X,Pair.Key.Y)
			BorderTileIndexes.Add(Pair.Key);
		}
	}
	if(bIsTemp)
	{
		AssaultRangeTiles.Empty();
	}
	else
	{
		TurnAssaultRangeTiles.Empty();
	}
	
	// AssaultRangeTiles.Empty();
	int MaxAtkDeviation = ParentPtr->GetMaxAtkDeviation();
	for(int i = 1;i <= ParentPtr->MaxAtkRange;i++)
	{
		for (const auto& BorderIndex : BorderTileIndexes)
		{
			// FIntPoint Direction = BorderIndex - ParentPtr->GridIndex;
			//如果是上下 左右 就往前扩一格
			FIntPoint Up(BorderIndex.X +i,BorderIndex.Y);
			const FTileData* BorderTileData = ParentPtr->MyGrid->GetTileDataByIndex(BorderIndex);

			if(const FTileData* UpTileData = ParentPtr->MyGrid->GetTileDataByIndex(Up))
			{
				const int Delta = FMathf::Abs(UpTileData->Height - BorderTileData->Height);
				if(Delta <= MaxAtkDeviation)
					AddAssaultRange(Up,bIsTemp);			
			}
			
			FIntPoint Right(BorderIndex.X ,BorderIndex.Y+i);
			if(const FTileData* RightTileData = ParentPtr->MyGrid->GetTileDataByIndex(Right))
			{
				const int Delta = FMathf::Abs(RightTileData->Height - BorderTileData->Height);
				if(Delta <= MaxAtkDeviation)
					AddAssaultRange(Right,bIsTemp);			
			}
			
			FIntPoint Left(BorderIndex.X ,BorderIndex.Y-i);
			if(const FTileData* LeftTileData = ParentPtr->MyGrid->GetTileDataByIndex(Left))
			{
				const int Delta = FMathf::Abs(LeftTileData->Height - BorderTileData->Height);
				if(Delta <= MaxAtkDeviation)
					AddAssaultRange(Left,bIsTemp);			
			}
			
			FIntPoint Down(BorderIndex.X - i,BorderIndex.Y);
			if(const FTileData* DownTileData = ParentPtr->MyGrid->GetTileDataByIndex(Down))
			{
				const int Delta = FMathf::Abs(DownTileData->Height - BorderTileData->Height);
				if(Delta <= MaxAtkDeviation)
					AddAssaultRange(Down,bIsTemp);			
			}
			
			// AddAssaultRange(Right,bIsTemp);
			// AddAssaultRange(Down,bIsTemp);
			// AddAssaultRange(Left,bIsTemp);
		}
	}
	if(!bIsTemp)
	{
		AssaultRangeTiles = TurnAssaultRangeTiles;
	}
	
	// UE_LOG(LogTemp,Log,TEXT("UUnitPathComponent::UnitAssaultRange() Completed"))
}



