// Fill out your copyright notice in the Description page of Project Settings.


#include "My_Utilities.h"

#include "Grid.h"
#include "UnitData.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "MyUnit.h"

My_Utilities::My_Utilities()
{
}

My_Utilities::~My_Utilities()
{
}
FVector SnapVectorToVector(const FVector& v1,const FVector& v2)
{
	float X = FMath::RoundHalfToEven(v1.X / v2.X) * v2.X;
	float Y = FMath::RoundHalfToEven(v1.Y / v2.Y) * v2.Y;
	float Z = FMath::RoundHalfToEven(v1.Z / v2.Z) * v2.Z;
	return FVector(X,Y,Z);
}

float SnapFloatToFloat(float f1,float f2)
{
	return FMath::RoundToInt(f1 / f2) * f2;
}

bool IsFloatEven(float f)
{
	return fmod(f ,2.0f) == 0.0;
}

bool IsIntEven(int i)
{
	return i % 2 ==0;
}


FUnitData* GetUnitData(const EUnitType unitType)
{
	static UDataTable* LoadedDataTable = nullptr;
	if(LoadedDataTable == nullptr)
	{
		FSoftObjectPath MyAssetPath(TEXT("DataTable'/Game/Art/Units/DT_DefaultUnitDataPerType.DT_DefaultUnitDataPerType'"));
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		LoadedDataTable = Cast<UDataTable>(Streamable.LoadSynchronous(MyAssetPath));
	}
	FUnitData* row = nullptr;
	switch (unitType)
	{
	case EUnitType::Warrior:
		row = LoadedDataTable->FindRow<FUnitData>(FName(TEXT("Warrior")),"");
		break;
	case EUnitType::Slime:
		row = LoadedDataTable->FindRow<FUnitData>(FName(TEXT("Slime")),"");
		break;
	case EUnitType::Ranger:
		row = LoadedDataTable->FindRow<FUnitData>(FName(TEXT("Ranger")),"");
		break;
	}
	return row;
}

FGridShapeData* GetShapeData(const EGridShape shape)
{
	static UDataTable* LoadedDataTable = nullptr;
	if(LoadedDataTable == nullptr)
	{
		
		FSoftObjectPath MyAssetPath(TEXT("DataTable'/Game/Core/Grid/GridShapes/DT_GridShapeData.DT_GridShapeData'"));
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		LoadedDataTable = Cast<UDataTable>(Streamable.LoadSynchronous(MyAssetPath));
	}
	
	FGridShapeData* row = nullptr;
	switch (shape)
	{
	case EGridShape::Hexagon:
		row = LoadedDataTable->FindRow<FGridShapeData>(FName(TEXT("Hexagon")),"");
		break;
	case EGridShape::Square:
		row = LoadedDataTable->FindRow<FGridShapeData>(FName(TEXT("Square")),"");
		break;
	case EGridShape::Triangle:
		row = LoadedDataTable->FindRow<FGridShapeData>(FName(TEXT("Triangle")),"");
		break;
	}
	return row;
}

bool IsTileTypeWalkable(ETileType  type)
{
	return !(type == ETileType::None || type == ETileType::Obstacle);
}



bool IsWrapAttack(TObjectPtr<AMyUnit> Attacker,TObjectPtr<AMyUnit> Defender,TObjectPtr<AMyUnit> Cooperator)
{//todo
	
	//包夹其实不在乎 参与包夹的角色朝向
	FVector AttackerForward = Attacker->GetActorForwardVector();
	FVector DefenderForward = Defender->GetActorForwardVector();
	//夹击协助者 必须站在 防御者的 上下左右四个相邻格子上
	FIntPoint DefenderIndex = Defender->GetGridIndex();
	FIntPoint Up = DefenderIndex.X + 1;
	FIntPoint Down = DefenderIndex.X - 1;
	FIntPoint Left = DefenderIndex.Y + 1;
	FIntPoint Right = DefenderIndex.Y - 1;

	if(Up == Cooperator->GetGridIndex() || Down == Cooperator->GetGridIndex()
		|| Left == Cooperator->GetGridIndex() || Right == Cooperator->GetGridIndex())
	{
		
	}
	
	return false;
}

bool IsBackAttack(TObjectPtr<AMyUnit> Attacker,TObjectPtr<AMyUnit> Defender)
{
	FVector AttackerForward = Attacker->GetActorForwardVector();
	FVector DefenderForward = Defender->GetActorForwardVector();

	FVector Result = AttackerForward.Cross(DefenderForward);
	return FMathf::Floor(Result.Length()) == 0;
}

/*
 *
* 命中率=90 ± 高低格h*2 + 夹击系数10（参与夹击的单位）+ 背击系数20 +2*（角色命中-角色闪避）
* 90+（6-8）*2=86%
* 敌人 8
* 主角 6
 */
int CalculateHitRate(TObjectPtr<AMyUnit> Attacker,TObjectPtr<AMyUnit> Defender,TObjectPtr<AGrid> MyGrid,bool IsWrap)
{
	FIntPoint AttackerGrid = Attacker->GetGridIndex();
	FIntPoint DefenderGrid = Defender->GetGridIndex();

	
	int HighDisparity = MyGrid->CalcHighDisparity(AttackerGrid,DefenderGrid);

	int Wrap = IsWrap ? 10 : 0;

	int BackAtk = IsBackAttack(Attacker,Defender) ? 20 : 0;

	
	int HitResult = 90 + HighDisparity + Wrap + BackAtk
	+ (Attacker->GetRuntimeProperty().HitProb - Defender->GetRuntimeProperty().HitProb) * 2;
	
	return HitResult;
}