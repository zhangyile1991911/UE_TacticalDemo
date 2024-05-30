#pragma once

#include "UnitRunTimeProperty.generated.h"

USTRUCT(BlueprintType)
struct FUnitRunTimeProperty
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int HP;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int ActionPoint;
	
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

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int UnitSide;//属于哪一个阵营
};
