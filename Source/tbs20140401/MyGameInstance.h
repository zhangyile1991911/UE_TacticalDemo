// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

enum class ETBSUnitType : uint8;
enum class EGridShape;
struct FUnitData;
struct FWeaponData;
struct FUnitColorDataAsset;
struct FStageData;

/**
 * 
 */
UCLASS()
class TBS20140401_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
protected:

	
	UPROPERTY()
	UDataTable* LoadedUnitDataTable = nullptr;
	
	UPROPERTY()
	UDataTable* LoadedColorDataTable = nullptr;

	UPROPERTY()
	UDataTable* LoadedStageDataTable = nullptr;

	UPROPERTY()
	UDataTable* LoadedWeaponDataTable = nullptr;
public:
	FUnitData* GetUnitData(const ETBSUnitType unitType);
	FWeaponData* GetWeaponData(FName WeaponId);
	FUnitColorDataAsset* GetUnitDataColor(const ETBSUnitType unitType);
	FStageData* GetStageData(int i);
};
