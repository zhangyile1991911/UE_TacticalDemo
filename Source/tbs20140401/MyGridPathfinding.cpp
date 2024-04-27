// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGridPathfinding.h"

#include "GridShapeData.h"
#include "Grid.h"
#include "My_Utilities.h"
#include "Kismet/GameplayStatics.h"

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
}

// Called every frame
void AMyGridPathfinding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
