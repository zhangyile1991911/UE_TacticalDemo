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


USTRUCT(BlueprintType)
struct FUnitProperty
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int WalkRange;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Attack;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Magic;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Agility;//行动速度

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int DistanceToAction;//行程

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int SkillPoint;//技能点 使用技能时候消耗
	
};
