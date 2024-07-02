// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAbility_Cooperate.h"

#include "BattleFunc.h"
#include "TileData.h"
#include "MyUnit.h"
#include "BattleReport.h"

AUnitAbility_Cooperate::AUnitAbility_Cooperate()
{
	
}

bool AUnitAbility_Cooperate::CanExecute()
{
	return true;
}

TArray<FIntPoint> AUnitAbility_Cooperate::Range(const FIntPoint& Int32Point)
{
	TArray<FIntPoint> Result;
	Result.Reserve(4);
	Result.Add(FIntPoint(Int32Point.X + 1,Int32Point.Y));
	Result.Add(FIntPoint(Int32Point.X - 1,Int32Point.Y));
	Result.Add(FIntPoint(Int32Point.X,Int32Point.Y + 1));
	Result.Add(FIntPoint(Int32Point.X,Int32Point.Y - 1));
	return MoveTemp(Result);
}

bool AUnitAbility_Cooperate::IsValidTarget(const FTileData* TileData,AGrid* MyGrid)
{
	if(TileData == nullptr)return false;
	if(TileData->UnitOnTile == nullptr)return false;
	if(TileData->UnitOnTile->IsFriend(OwnerInstance->GetUnitSide()))return false;
	return true;
}

TArray<TObjectPtr<AMyUnit>> AUnitAbility_Cooperate::TakeTargets(const FIntPoint& Point, AGrid* MyGrid)
{
	return Super::TakeTargets(Point, MyGrid);
}

FBattleReport AUnitAbility_Cooperate::DoCalculation(const TArray<TObjectPtr<AMyUnit>>& Targets, AGrid* MyGrid,bool NeedCooperator)
{
	return DoCalculation(Targets[0],MyGrid,NeedCooperator);
}

FBattleReport AUnitAbility_Cooperate::DoCalculation(TObjectPtr<AMyUnit> Target, AGrid* MyGrid,bool NeedCooperator)
{
	FBattleReport Report;
	Report.Attacker = OwnerInstance;
	Report.Defender = Target;
	Report.IsCritical = UBattleFunc::IsCritical(OwnerInstance,Target);;
	Report.Cooperator = nullptr;
	Report.IsBackAtk = UBattleFunc::IsBackAttack(OwnerInstance,Target);
	Report.HitPercent = UBattleFunc::CalculateHitRate(OwnerInstance,Target,MyGrid,false,Report.IsBackAtk);

	int Num = FMath::RandRange(0,100);
	if(Num > Report.HitPercent)
	{//未命中
		Report.Damage = 0;
		Report.IsHit = false;
		return MoveTemp(Report);	
	}

	Report.IsHit = true;
	float atk = OwnerInstance->GetRuntimeProperty().Power;
	float def = Target->GetRuntimeProperty().PhysicDefend;
	float per = FMath::FRandRange(0.8,1.0f);
	Report.Damage = 10;//FMathf::Clamp(atk * per - def,0,999999999);
	//扣除血量
	Target->AddHP(Report.Damage);
	
	return MoveTemp(Report);
}

