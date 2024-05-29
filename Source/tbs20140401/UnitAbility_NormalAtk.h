// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitAbility.h"
#include "BattleReport.h"
#include "UnitAbility_NormalAtk.generated.h"

UCLASS()
class TBS20140401_API UUnitAbility_NormalAtk : public UUnitAbility
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UUnitAbility_NormalAtk();

protected:
	// Called when the game starts or when spawned
	// virtual void BeginPlay() override;

public:
	// Called every frame
	// virtual void Tick(float DeltaTime) override;
	virtual bool CanExecute() override;
	virtual TArray<FIntPoint> Range(const FIntPoint&) override;
	virtual bool IsValidTarget(const FTileData& TileData) override;
	virtual TArray<TObjectPtr<AMyUnit>> TakeTargets(const FIntPoint& Point, AGrid* MyGrid) override;
	virtual FBattleReport DoCalculation(const TArray<TObjectPtr<AMyUnit>>& Targets,AGrid* MyGrid) override;
	virtual FBattleReport DoCalculation(TObjectPtr<AMyUnit> Target, AGrid* MyGrid) override;
};
