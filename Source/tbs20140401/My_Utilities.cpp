// Fill out your copyright notice in the Description page of Project Settings.


#include "My_Utilities.h"

#include <ThirdParty/SPIRV-Reflect/SPIRV-Reflect/include/spirv/unified1/spirv.h>

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


FUnitData* GetUnitData(const ETBSUnitType unitType)
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
	case ETBSUnitType::Warrior:
		row = LoadedDataTable->FindRow<FUnitData>(FName(TEXT("Warrior")),"");
		break;
	case ETBSUnitType::Slime:
		row = LoadedDataTable->FindRow<FUnitData>(FName(TEXT("Slime")),"");
		break;
	case ETBSUnitType::Ranger:
		row = LoadedDataTable->FindRow<FUnitData>(FName(TEXT("Ranger")),"");
		break;
	case ETBSUnitType::Priest:
		row = LoadedDataTable->FindRow<FUnitData>(FName(TEXT("Priest")),"");
		break;
	case ETBSUnitType::Chicken:
		row = LoadedDataTable->FindRow<FUnitData>(FName(TEXT("Chicken")),"");
		break;
	case ETBSUnitType::Bat:
		row = LoadedDataTable->FindRow<FUnitData>(FName(TEXT("Bat")),"");
		break;
	case ETBSUnitType::EnemyWarrior:
		row = LoadedDataTable->FindRow<FUnitData>(FName(TEXT("EnemyWarrior")),"");
		break;
	case ETBSUnitType::EnemySlime:
		row = LoadedDataTable->FindRow<FUnitData>(FName(TEXT("EnemySlime")),"");
		break;
	case ETBSUnitType::EnemyRanger:
		row = LoadedDataTable->FindRow<FUnitData>(FName(TEXT("EnemyRanger")),"");
		break;
	case ETBSUnitType::EnemyTank:
		row = LoadedDataTable->FindRow<FUnitData>(FName(TEXT("EnemyTank")),"");
		break;
	case ETBSUnitType::EnemyBat:
		row = LoadedDataTable->FindRow<FUnitData>(FName(TEXT("EnemyBat")),"");
		break;
	case ETBSUnitType::EnemyChicken:
		row = LoadedDataTable->FindRow<FUnitData>(FName(TEXT("EnemyChicken")),"");
		break;
	case ETBSUnitType::EnemyPriest:
		row = LoadedDataTable->FindRow<FUnitData>(FName(TEXT("EnemyPriest")),"");
		break;
	}
	return row;
}

FUnitColorDataAsset* GetUnitDataColor(const ETBSUnitType unitType)
{
	static UDataTable* LoadedDataTable = nullptr;
	if(LoadedDataTable == nullptr)
	{
		FSoftObjectPath MyAssetPath(TEXT("DataTable'/Game/Art/Units/DT_UnitColor.DT_UnitColor'"));
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		LoadedDataTable = Cast<UDataTable>(Streamable.LoadSynchronous(MyAssetPath));
	}
	FUnitColorDataAsset* row = nullptr;
	switch (unitType)
	{
	case ETBSUnitType::Warrior:
		row = LoadedDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("Warrior")),"");
		break;
	case ETBSUnitType::Slime:
		row = LoadedDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("Slime")),"");
		break;
	case ETBSUnitType::Ranger:
		row = LoadedDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("Ranger")),"");
		break;
	case ETBSUnitType::Priest:
		row = LoadedDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("Priest")),"");
		break;
	case ETBSUnitType::Bat:
		row = LoadedDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("Bat")),"");
		break;
	case ETBSUnitType::Chicken:
		row = LoadedDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("Chicken")),"");
		break;
	case ETBSUnitType::EnemyWarrior:
		row = LoadedDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("EnemyWarrior")),"");
		break;
	case ETBSUnitType::EnemySlime:
		row = LoadedDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("EnemySlime")),"");
		break;
	case ETBSUnitType::EnemyRanger:
		row = LoadedDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("EnemyRanger")),"");
		break;
	case ETBSUnitType::EnemyTank:
		row = LoadedDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("EnemyTank")),"");
		break;
	case ETBSUnitType::EnemyPriest:
		row = LoadedDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("EnemyPriest")),"");
		break;
	case ETBSUnitType::EnemyChicken:
		row = LoadedDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("EnemyChicken")),"");
		break;
	case ETBSUnitType::EnemyBat:
		row = LoadedDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("EnemyBat")),"");
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

FWeaponData* GetWeaponData(FName WeaponId)
{
	static UDataTable* LoadedDataTable = nullptr;
	if(LoadedDataTable == nullptr)
	{
		FSoftObjectPath MyAssetPath(TEXT("DataTable'/Game/Art/Units/DT_WeaponData.DT_WeaponData'"));
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		LoadedDataTable = Cast<UDataTable>(Streamable.LoadSynchronous(MyAssetPath));
	}
	FWeaponData* row = LoadedDataTable->FindRow<FWeaponData>(WeaponId,"");
	return row;
}

bool IsTileTypeWalkable(ETileType  type)
{
	return !(type == ETileType::None || type == ETileType::Obstacle);
}



