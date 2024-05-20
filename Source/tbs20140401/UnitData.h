﻿#pragma once
#include "CoreMinimal.h"
#include "UnitDataAsset.h"
#include "UnitData_Stats.h"
#include "UnitData.generated.h"

UENUM(BlueprintType)
enum class EUnitType:uint8
{
	Warrior UMETA(DisplayName="Warrior"),
	Ranger UMETA(DisplayName="Ranger"),
	Slime UMETA(DisplayName="Slime"),
	// Priest UMETA(DisplayName="Priest"),
	// Chicken UMETA(DisplayName="Chicken"),
	// Bat UMETA(DisplayName="Bat") 
};

USTRUCT(BlueprintType)
struct FUnitData:public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EUnitType Type;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FUnitDataAsset Assets;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FUnitData_Stats Stats;
};