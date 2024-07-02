// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitAbility.h"
#include "BattleReport.h"
#include "UnitAbilityAnim.h"
#include "UnitAbility_NormalAtk.generated.h"


UCLASS(Blueprintable)
class TBS20140401_API AUnitAbility_NormalAtk : public AUnitAbilityAnim
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUnitAbility_NormalAtk();

protected:
	// Called when the game starts or when spawned
	// virtual void BeginPlay() override;

public:
	// Called every frame
	// virtual void Tick(float DeltaTime) override;
	virtual bool CanExecute() override;
	virtual TArray<FIntPoint> Range(const FIntPoint&) override;
	virtual bool IsValidTarget(const FTileData* TileData,AGrid* MyGrid) override;
	virtual bool IsValidUnit(TObjectPtr<AMyUnit> Unit) override;
	virtual TArray<TObjectPtr<AMyUnit>> TakeTargets(const FIntPoint& Point, AGrid* MyGrid) override;
	virtual FBattleReport DoCalculation(const TArray<TObjectPtr<AMyUnit>>& Targets,AGrid* MyGrid,bool NeedCooperator) override;
	virtual FBattleReport DoCalculation(TObjectPtr<AMyUnit> Target, AGrid* MyGrid,bool NeedCooperator) override;
};
