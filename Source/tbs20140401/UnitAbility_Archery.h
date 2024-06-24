// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitAbilityAnim.h"
#include "UnitAbility_Archery.generated.h"

UCLASS()
class TBS20140401_API AUnitAbility_Archery : public AUnitAbilityAnim
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUnitAbility_Archery();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool IsArea()override{return false;}
	virtual bool CanExecute() override;
	virtual TArray<FIntPoint> Range(const FIntPoint&) override;
	virtual bool IsValidTarget(const FTileData& TileData,AGrid* MyGrid) override;
	virtual bool IsValidUnit(TObjectPtr<AMyUnit> Unit) override;
	virtual TArray<FIntPoint> Indicator(const FIntPoint& Index)override;
	virtual TArray<TObjectPtr<AMyUnit>> TakeTargets(const FIntPoint& Point, AGrid* MyGrid) override;
	virtual FBattleReport DoCalculation(const TArray<TObjectPtr<AMyUnit>>& Targets,AGrid* MyGrid,bool NeedCooperator) override;
	virtual FBattleReport DoCalculation(TObjectPtr<AMyUnit> Target, AGrid* MyGrid,bool NeedCooperator) override;
};
