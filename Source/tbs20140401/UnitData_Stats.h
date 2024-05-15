#pragma once

#include "CoreMinimal.h"
#include "GridShapeData.h"
#include "UnitData_Stats.generated.h"

USTRUCT(BlueprintType)
struct FUnitData_Stats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<ETileType> ValidTileTypes;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool CanMoveDiagonally;
};
