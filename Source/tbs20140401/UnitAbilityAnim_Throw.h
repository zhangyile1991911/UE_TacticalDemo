// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitAbilityAnim.h"
#include "UnitAbilityAnim_Throw.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TBS20140401_API AUnitAbilityAnim_Throw : public AUnitAbilityAnim
{
	GENERATED_BODY()
public:
	AUnitAbilityAnim_Throw();
protected:
	
public:
	virtual bool CanExecute() override;
	virtual TArray<FIntPoint> Range(const FIntPoint&) override;
	virtual bool IsValidTarget(const FTileData* TileData,AGrid* MyGrid) override;
	virtual bool IsValidUnit(TObjectPtr<AMyUnit> Unit) override;
	virtual TArray<TObjectPtr<AMyUnit>> TakeTargets(const FIntPoint& Point, AGrid* MyGrid) override;
	virtual FBattleReport DoCalculation(const TArray<TObjectPtr<AMyUnit>>& Targets,AGrid* MyGrid,bool NeedCooperator) override;
	virtual FBattleReport DoCalculation(TObjectPtr<AMyUnit> Target, AGrid* MyGrid,bool NeedCooperator) override;
};
