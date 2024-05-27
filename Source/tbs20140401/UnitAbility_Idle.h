// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitAbility.h"
#include "UnitAbility_Idle.generated.h"

/**
 * 
 */
UCLASS()
class TBS20140401_API UUnitAbility_Idle : public UUnitAbility
{
	GENERATED_BODY()
public:
	virtual bool CanExecute()override;
	virtual bool IsValidTarget(const FTileData& TileData) override;
	virtual void Execute()override;
};
