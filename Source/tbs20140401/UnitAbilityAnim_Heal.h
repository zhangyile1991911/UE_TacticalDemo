// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitAbilityAnim.h"
#include "Curve/CurveUtil.h"
#include "UnitAbilityAnim_Heal.generated.h"

UCLASS()
class TBS20140401_API AUnitAbilityAnim_Heal : public AUnitAbilityAnim
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUnitAbilityAnim_Heal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	// virtual void Tick(float DeltaTime) override;
	virtual bool CanExecute() override;
	virtual TArray<FIntPoint> Range(const FIntPoint&) override;
	virtual TArray<FIntPoint> Indicator(const FIntPoint& Index) override;
	virtual bool IsValidTarget(const FTileData& TileData,AGrid* MyGrid) override;
	virtual bool IsValidUnit(TObjectPtr<AMyUnit> Unit) override;
	virtual TArray<TObjectPtr<AMyUnit>> TakeTargets(const FIntPoint& Point, AGrid* MyGrid) override;
	virtual FBattleReport DoCalculation(const TArray<TObjectPtr<AMyUnit>>& Targets,AGrid* MyGrid,bool NeedCooperator) override;
	virtual FBattleReport DoCalculation(TObjectPtr<AMyUnit> Target, AGrid* MyGrid,bool NeedCooperator) override;
};
