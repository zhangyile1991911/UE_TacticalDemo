// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GridDataHelper.generated.h"

struct FGridInfoSave;
/**
 * 
 */
UCLASS()
class TBS20140401_API UGridDataHelper : public UObject
{
	GENERATED_BODY()

public:
	static bool SaveGridToJson(const FGridInfoSave& GridData, const FString& FilePath);

	static bool LoadGridFromJson(FGridInfoSave& GridData, const FString& FilePath);
};
