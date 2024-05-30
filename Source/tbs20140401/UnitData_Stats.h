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
	int HP;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Agility;//行动条上的移动速度

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int DistanceToAction;//行动条的行程

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int ActionPoint;//技能点 使用技能时候消耗
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Power;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int PhysicDefend;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int MagicPower;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int MagicDefend;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Lucky;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int HitProb;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Speed;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Dodge;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Jump;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Move;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int FireResistance;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int IceResistance;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int WindResistance;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int ThunderResistance;
};
