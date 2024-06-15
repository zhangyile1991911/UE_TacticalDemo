// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FunitColorDataAsset.h"
#include "GridShapeData.h"
#include "UnitData.h"

/**
 * 
 */
class TBS20140401_API My_Utilities
{
public:
	My_Utilities();
	~My_Utilities();
};

class AMyUnit;
class AGrid;
FVector SnapVectorToVector(const FVector& v1,const FVector& v2);
float SnapFloatToFloat(float f1,float f2);
bool IsFloatEven(float f);
bool IsIntEven(int i);
FUnitData* GetUnitData(const EUnitType unitType);
FGridShapeData* GetShapeData(EGridShape shapeName);
bool IsTileTypeWalkable(ETileType);
FUnitColorDataAsset* GetUnitDataColor(const EUnitType unitType);
