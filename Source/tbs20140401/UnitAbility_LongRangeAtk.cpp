﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAbility_LongRangeAtk.h"

#include "BattleFunc.h"
#include "BattleReport.h"
#include "Grid.h"
#include "MyUnit.h"
#include "TileData.h"

AUnitAbility_LongRangeAtk::AUnitAbility_LongRangeAtk()
{
	
}

bool AUnitAbility_LongRangeAtk::CanExecute()
{
	return true;
}

TArray<FIntPoint> AUnitAbility_LongRangeAtk::Range(const FIntPoint& Int32Point)
{
	TArray<FIntPoint> Result;
	Result.Reserve(16);
	for(int i = SkillData.Range.X;i <= SkillData.Range.Y;i++)
	{
		Result.Add(FIntPoint(Int32Point.X+i,Int32Point.Y));
		Result.Add(FIntPoint(Int32Point.X-i,Int32Point.Y));
		Result.Add(FIntPoint(Int32Point.X,Int32Point.Y+i));
		Result.Add(FIntPoint(Int32Point.X,Int32Point.Y-i));
	}
	return Result;
}

bool AUnitAbility_LongRangeAtk::IsValidTarget(const FTileData& TileData,AGrid* MyGrid)
{
	if(TileData.UnitOnTile == nullptr)return false;
	if(TileData.UnitOnTile->IsDead())return false;
	if(TileData.UnitOnTile->IsFriend(OwnerInstance->GetUnitSide()))return false;
	return true;
}

bool AUnitAbility_LongRangeAtk::IsValidUnit(TObjectPtr<AMyUnit> Unit)
{
	if(Unit == nullptr)return false;
	if(Unit->IsDead())return false;
	if(Unit->GetRuntimeProperty().UnitSide == OwnerInstance->GetRuntimeProperty().UnitSide)return false;
	return true;
}

TArray<FIntPoint> AUnitAbility_LongRangeAtk::Indicator(const FIntPoint& Index)
{
	FVector2D Dir;
	if(OwnerInstance->NeedToMove())
	{
		Dir = Index - OwnerInstance->GetTempDestinationGridIndex();
	}
	else
	{
		Dir = Index - OwnerInstance->GetGridIndex();
	}
	Dir = Dir.GetSafeNormal();
    int X = Dir.X;
    int Y = Dir.Y;
    FIntPoint Tmp = OwnerInstance->GetGridIndex();
	TArray<FIntPoint> IndicatorRange;
	IndicatorRange.Reserve(SkillData.Range.Y);
    for(int i = SkillData.Range.X;i <= SkillData.Range.Y;i++)
    {
    	Tmp.X += X;
    	Tmp.Y += Y;
    	IndicatorRange.Add(Tmp);
    }
	return IndicatorRange;
}

TArray<TObjectPtr<AMyUnit>> AUnitAbility_LongRangeAtk::TakeTargets(const FIntPoint& Point, AGrid* MyGrid)
{
	TArray<TObjectPtr<AMyUnit>> Targets;
	FVector2D Dir;
	if(OwnerInstance->NeedToMove())
	{
		Dir = Point - OwnerInstance->GetTempDestinationGridIndex();
	}
	else
	{
		Dir = Point - OwnerInstance->GetGridIndex();
	}
	Dir = Dir.GetSafeNormal();
	int X = Dir.X;
	int Y = Dir.Y;
	FIntPoint Tmp = OwnerInstance->GetGridIndex();
	for(int i = SkillData.Range.X;i <= SkillData.Range.Y;i++)
	{
		Tmp.X += X;
		Tmp.Y += Y;
		auto Unit = MyGrid->GetUnitOnTile(Tmp);
		if(Unit != nullptr)Targets.Add(Unit);
	}
	return MoveTemp(Targets);
}

FBattleReport AUnitAbility_LongRangeAtk::DoCalculation(const TArray<TObjectPtr<AMyUnit>>& Targets, AGrid* MyGrid,bool NeedCooperator)
{
	FBattleReport Report;
	Report.Attacker = OwnerInstance;
	
	for (int i = 0; i < Targets.Num();i++)
	{
		FHitInfo HitInfo;
		auto OneTarget = Targets[i];
		HitInfo.Defender = OneTarget;
		HitInfo.Cooperator = UBattleFunc::HasWrapAttackUnit(OwnerInstance,OneTarget,MyGrid);
		if(HitInfo.Cooperator != nullptr)HitInfo.CooperatorTarget = OneTarget;
		
		HitInfo.IsCritical = UBattleFunc::IsCritical(OwnerInstance,OneTarget);
		HitInfo.IsBackAtk = UBattleFunc::IsBackAttack(OwnerInstance,OneTarget);
		HitInfo.HitPercent = UBattleFunc::CalculateHitRate(OwnerInstance,OneTarget,MyGrid,HitInfo.Cooperator != nullptr,Report.IsBackAtk);

		int Num = FMath::RandRange(0,100);
		if(Num > HitInfo.HitPercent)
		{//未命中
			HitInfo.Damage = 0;
			HitInfo.IsHit = false;
		}
		else
		{
			HitInfo.IsHit = true;
			float atk = OwnerInstance->GetRuntimeProperty().Power;
			float def = OneTarget->GetRuntimeProperty().PhysicDefend;
			float per = FMath::FRandRange(0.8,1.0f);
			HitInfo.Damage = 10;
			OneTarget->AddHP(HitInfo.Damage);
		}
		Report.HitInfoList.Add(HitInfo);
	}
	return MoveTemp(Report);
}

FBattleReport AUnitAbility_LongRangeAtk::DoCalculation(TObjectPtr<AMyUnit> Target, AGrid* MyGrid, bool NeedCooperator)
{
	return Super::DoCalculation(Target, MyGrid, NeedCooperator);
}