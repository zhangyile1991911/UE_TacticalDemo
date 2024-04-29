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
