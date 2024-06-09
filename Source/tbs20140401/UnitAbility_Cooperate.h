// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitAbilityAnim.h"
#include "UnitAbility_Cooperate.generated.h"

/**
 * 
 */
UCLASS()
class TBS20140401_API AUnitAbility_Cooperate : public AUnitAbilityAnim
{

	GENERATED_BODY()
public:
	AUnitAbility_Cooperate();

	virtual bool IsShowOnCmd()override{return false;}
	virtual bool IsCooperate()override{return true;}
	virtual bool CanExecute() override;
	virtual TArray<FIntPoint> Range(const FIntPoint&) override;
	virtual bool IsValidTarget(const FTileData& TileData,AGrid* MyGrid) override;
	virtual TArray<TObjectPtr<AMyUnit>> TakeTargets(const FIntPoint& Point, AGrid* MyGrid) override;
	virtual FBattleReport DoCalculation(const TArray<TObjectPtr<AMyUnit>>& Targets,AGrid* MyGrid,bool NeedCooperator) override;
	virtual FBattleReport DoCalculation(TObjectPtr<AMyUnit> Target, AGrid* MyGrid,bool NeedCooperator) override;

};
