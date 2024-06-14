#pragma once

#include "MyPathFindingData.generated.h"
USTRUCT(BlueprintType)
struct FMyPathFindingData
{
	GENERATED_BODY()
	
	FIntPoint Index = FIntPoint(-999,-999);
	int CostToEnterTile = 1;
	int CostFromStart = 999999;//G
	int MinimumCostToTarget = 999999;//H
	
	FIntPoint PreviousIndex = FIntPoint(-999,-999);
};

// USTRUCT(BlueprintType)
// struct FUnitRangeData
// {
// 	GENERATED_BODY()
// 	
// 	FIntPoint Index = FIntPoint(-999,-999);
// 	int Cost = 0;
// 	FUnitRangeData* Parent;
// 	TArray<FUnitRangeData> Children;
// 	int RangeType = 0;//1 可以走 2 可以攻击
// 	bool IsOccupied = false;
// };