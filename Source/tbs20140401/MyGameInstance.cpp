// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

#include "UnitData.h"
#include "FunitColorDataAsset.h"
#include "FStageData.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

FUnitData* UMyGameInstance::GetUnitData(const ETBSUnitType unitType)
{
	if(LoadedUnitDataTable == nullptr)
	{
		FSoftObjectPath MyAssetPath(TEXT("DataTable'/Game/Art/Units/DT_DefaultUnitDataPerType.DT_DefaultUnitDataPerType'"));
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		LoadedUnitDataTable = Cast<UDataTable>(Streamable.LoadSynchronous(MyAssetPath));
	}
	FUnitData* row = nullptr;
	switch (unitType)
	{
	case ETBSUnitType::Warrior:
		row = LoadedUnitDataTable->FindRow<FUnitData>(FName(TEXT("Warrior")),"");
		break;
	case ETBSUnitType::Slime:
		row = LoadedUnitDataTable->FindRow<FUnitData>(FName(TEXT("Slime")),"");
		break;
	case ETBSUnitType::Ranger:
		row = LoadedUnitDataTable->FindRow<FUnitData>(FName(TEXT("Ranger")),"");
		break;
	case ETBSUnitType::Priest:
		row = LoadedUnitDataTable->FindRow<FUnitData>(FName(TEXT("Priest")),"");
		break;
	case ETBSUnitType::Chicken:
		row = LoadedUnitDataTable->FindRow<FUnitData>(FName(TEXT("Chicken")),"");
		break;
	case ETBSUnitType::Bat:
		row = LoadedUnitDataTable->FindRow<FUnitData>(FName(TEXT("Bat")),"");
		break;
	case ETBSUnitType::EnemyWarrior:
		row = LoadedUnitDataTable->FindRow<FUnitData>(FName(TEXT("EnemyWarrior")),"");
		break;
	case ETBSUnitType::EnemySlime:
		row = LoadedUnitDataTable->FindRow<FUnitData>(FName(TEXT("EnemySlime")),"");
		break;
	case ETBSUnitType::EnemyRanger:
		row = LoadedUnitDataTable->FindRow<FUnitData>(FName(TEXT("EnemyRanger")),"");
		break;
	case ETBSUnitType::EnemyTank:
		row = LoadedUnitDataTable->FindRow<FUnitData>(FName(TEXT("EnemyTank")),"");
		break;
	case ETBSUnitType::EnemyBat:
		row = LoadedUnitDataTable->FindRow<FUnitData>(FName(TEXT("EnemyBat")),"");
		break;
	case ETBSUnitType::EnemyChicken:
		row = LoadedUnitDataTable->FindRow<FUnitData>(FName(TEXT("EnemyChicken")),"");
		break;
	case ETBSUnitType::EnemyPriest:
		row = LoadedUnitDataTable->FindRow<FUnitData>(FName(TEXT("EnemyPriest")),"");
		break;
	}
	return row;
	
}


FWeaponData* UMyGameInstance::GetWeaponData(FName WeaponId)
{
	if(LoadedWeaponDataTable == nullptr)
	{
		FSoftObjectPath MyAssetPath(TEXT("DataTable'/Game/Art/Units/DT_WeaponData.DT_WeaponData'"));
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		LoadedWeaponDataTable = Cast<UDataTable>(Streamable.LoadSynchronous(MyAssetPath));
	}
	FWeaponData* row = LoadedWeaponDataTable->FindRow<FWeaponData>(WeaponId,"");
	return row;
}

FUnitColorDataAsset* UMyGameInstance::GetUnitDataColor(const ETBSUnitType unitType)
{
	if(LoadedColorDataTable == nullptr)
	{
		FSoftObjectPath MyAssetPath(TEXT("DataTable'/Game/Art/Units/DT_UnitColor.DT_UnitColor'"));
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		LoadedColorDataTable = Cast<UDataTable>(Streamable.LoadSynchronous(MyAssetPath));
	}
	FUnitColorDataAsset* row = nullptr;
	switch (unitType)
	{
	case ETBSUnitType::Warrior:
		row = LoadedColorDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("Warrior")),"");
		break;
	case ETBSUnitType::Slime:
		row = LoadedColorDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("Slime")),"");
		break;
	case ETBSUnitType::Ranger:
		row = LoadedColorDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("Ranger")),"");
		break;
	case ETBSUnitType::Priest:
		row = LoadedColorDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("Priest")),"");
		break;
	case ETBSUnitType::Bat:
		row = LoadedColorDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("Bat")),"");
		break;
	case ETBSUnitType::Chicken:
		row = LoadedColorDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("Chicken")),"");
		break;
	case ETBSUnitType::EnemyWarrior:
		row = LoadedColorDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("EnemyWarrior")),"");
		break;
	case ETBSUnitType::EnemySlime:
		row = LoadedColorDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("EnemySlime")),"");
		break;
	case ETBSUnitType::EnemyRanger:
		row = LoadedColorDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("EnemyRanger")),"");
		break;
	case ETBSUnitType::EnemyTank:
		row = LoadedColorDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("EnemyTank")),"");
		break;
	case ETBSUnitType::EnemyPriest:
		row = LoadedColorDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("EnemyPriest")),"");
		break;
	case ETBSUnitType::EnemyChicken:
		row = LoadedColorDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("EnemyChicken")),"");
		break;
	case ETBSUnitType::EnemyBat:
		row = LoadedColorDataTable->FindRow<FUnitColorDataAsset>(FName(TEXT("EnemyBat")),"");
		break;
	}
	return row;
}

FStageData UMyGameInstance::GetStageData(int i)
{
	if(LoadedStageDataTable == nullptr)
	{
		FSoftObjectPath MyAssetPath(TEXT("DataTable'/Game/Demo/DT_StageData.DT_StageData'"));
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		LoadedStageDataTable = Cast<UDataTable>(Streamable.LoadSynchronous(MyAssetPath));
	}

	FString RowName = FString::Format(TEXT("{0}"),{i});
	FStageData* row = LoadedStageDataTable->FindRow<FStageData>(FName(RowName),"");
	return *row;
}
