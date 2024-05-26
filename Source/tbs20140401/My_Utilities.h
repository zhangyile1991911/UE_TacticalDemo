// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

bool IsBackAttack(TObjectPtr<AMyUnit> Attacker,TObjectPtr<AMyUnit> Defender);
bool IsWrapAttack(TObjectPtr<AMyUnit> Attacker,TObjectPtr<AMyUnit> Defender,TObjectPtr<AMyUnit> Cooperator);
int CalculateHitRate(TObjectPtr<AMyUnit> Attacker,TObjectPtr<AMyUnit> Defender,TObjectPtr<AGrid> MyGrid,bool IsWrap);