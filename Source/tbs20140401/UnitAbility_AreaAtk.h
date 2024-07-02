// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitAbilityAnim.h"
#include "UnitAbility_AreaAtk.generated.h"

UCLASS(Blueprintable)
class TBS20140401_API AUnitAbility_AreaAtk : public AUnitAbilityAnim
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AMyGridPathfinding> MyGridPathfinding;
public:
	// Sets default values for this actor's properties
	AUnitAbility_AreaAtk();
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual bool CanExecute() override;
	virtual TArray<FIntPoint> Range(const FIntPoint&) override;
	virtual TArray<FIntPoint> Indicator(const FIntPoint& Index)override;
	virtual bool IsArea()override{return true;}
	virtual bool IsValidTarget(const FTileData* TileData,AGrid* MyGrid) override;
	virtual bool IsValidUnit(TObjectPtr<AMyUnit> Unit) override;
	virtual TArray<TObjectPtr<AMyUnit>> TakeTargets(const FIntPoint& Point, AGrid* MyGrid) override;
	virtual FBattleReport DoCalculation(const TArray<TObjectPtr<AMyUnit>>& Targets,AGrid* MyGrid,bool NeedCooperator) override;
	virtual FBattleReport DoCalculation(TObjectPtr<AMyUnit> Target, AGrid* MyGrid,bool NeedCooperator) override;
};
