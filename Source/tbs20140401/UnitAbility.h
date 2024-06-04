// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleReport.h"
#include "SkillData.h"
#include "UObject/Object.h"
#include "UnitAbility.generated.h"


class AMy_Pawn;
class AGrid;


struct FTileData;
class AMyUnit;
/**
 * 
 */
UCLASS(Abstract)
class TBS20140401_API UUnitAbility : public UObject
{
	GENERATED_BODY()
// protected:
// 	virtual void BeginDestroy() override;
//
// protected:
// 	UPROPERTY(EditAnywhere,BlueprintReadOnly)
// 	TObjectPtr<AMyUnit> OwnerInstance;
// 	UPROPERTY(EditAnywhere,BlueprintReadOnly)
// 	FSkillData SkillData;
//
// 	// UPROPERTY()
// 	// TObjectPtr<UChildActorComponent> Animation;
// public:
// 	
// 	
// 	const FSkillData& GetSkillData()
// 	{
// 		return SkillData;
// 	}
// 	void SetSkillData(FSkillData Data,TObjectPtr<AMyUnit> OwnerInst)
// 	{
// 		SkillData = Data;
// 		OwnerInstance = OwnerInst;
// 	}
// 	const FText& GetAbilityName()const{return SkillData.SkillName;}
// 	int GetCost()const{return SkillData.SpendPoint;}
// 	
// 	virtual bool CanExecute(){return false;}
// 	virtual bool IsValidTarget(const FTileData& TileData){return false;}
// 	
// 	virtual FBattleReport DoCalculation(const TArray<TObjectPtr<AMyUnit>>& Targets,AGrid* MyGrid);
// 	virtual FBattleReport DoCalculation(TObjectPtr<AMyUnit> Target,AGrid* MyGrid);
// 	
// 	virtual TArray<FIntPoint> Range(const FIntPoint&){ return TArray<FIntPoint>();}
// 	virtual TArray<TObjectPtr<AMyUnit>> TakeTargets(const FIntPoint& Point,AGrid* MyGrid);
//
// 	virtual bool IsIdle(){return false;}
//
};
