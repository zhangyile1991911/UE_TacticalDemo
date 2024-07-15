// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAbility_NormalAtk.h"
#include "BattleFunc.h"
#include "MyUnit.h"
#include "Grid.h"


// Sets default values
AUnitAbility_NormalAtk::AUnitAbility_NormalAtk()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
// void UUnitAbility_NormalAtk::BeginPlay()
// {
// 	Super::BeginPlay();
// 	
// }

// Called every frame
// void UUnitAbility_NormalAtk::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// }

bool AUnitAbility_NormalAtk::CanExecute()
{
	return OwnerInstance->HasEnoughAP(SkillData.SpendPoint);
}

TArray<FIntPoint> AUnitAbility_NormalAtk::Range(const FIntPoint& Int32Point)
{
	TArray<FIntPoint> Result;
	Result.Reserve(4);
	Result.Add(FIntPoint(Int32Point.X + 1,Int32Point.Y));
	Result.Add(FIntPoint(Int32Point.X - 1,Int32Point.Y));
	Result.Add(FIntPoint(Int32Point.X,Int32Point.Y + 1));
	Result.Add(FIntPoint(Int32Point.X,Int32Point.Y - 1));
	return MoveTemp(Result);
}

bool AUnitAbility_NormalAtk::IsValidTarget(const FTileData* TileData,AGrid* MyGrid)
{
	if(TileData == nullptr)return false;
	if(TileData->UnitOnTile == nullptr)return false;
	if(TileData->UnitOnTile->IsDead())return false;
	if(TileData->UnitOnTile->IsFriend(OwnerInstance->GetUnitSide()))return false;
	
	return CheckTileDataHeight(TileData,MyGrid);
}

bool AUnitAbility_NormalAtk::IsValidUnit(TObjectPtr<AMyUnit> Unit)
{
	if(Unit == nullptr)return false;
	if(Unit->IsDead())return false;
	if(Unit->IsFriend(OwnerInstance->GetUnitSide()))return false;
	return true;
}

TArray<TObjectPtr<AMyUnit>> AUnitAbility_NormalAtk::TakeTargets(const FIntPoint& Point, AGrid* MyGrid)
{
	TArray<TObjectPtr<AMyUnit>> Targets;
	const FTileData* TileDataPtr = MyGrid->GetTileDataByIndex(Point);

	do
	{
		if(TileDataPtr == nullptr)break;
		if(TileDataPtr->UnitOnTile == nullptr)break;
		if(TileDataPtr->UnitOnTile->IsFriend(OwnerInstance->GetUnitSide()))break;
		if(!CheckTileDataHeight(TileDataPtr,MyGrid))break;
		Targets.Add(TileDataPtr->UnitOnTile);
	}
	while (false);
	
	return MoveTemp(Targets);
}

FBattleReport AUnitAbility_NormalAtk::DoCalculation(const TArray<TObjectPtr<AMyUnit>>& Targets, AGrid* MyGrid,bool NeedCooperator)
{
	// Super::DoCalculation(Targets, MyGrid,NeedCooperator);
	
	return DoCalculation(Targets[0],MyGrid,NeedCooperator);
}

FBattleReport AUnitAbility_NormalAtk::DoCalculation(TObjectPtr<AMyUnit> Target, AGrid* MyGrid,bool NeedCooperator)
{
	// Super::DoCalculation(Target, MyGrid,NeedCooperator);
	// TArray<FBattleReport> ReportList;
	TObjectPtr<AMyUnit> Cooperator = NeedCooperator ? UBattleFunc::HasWrapAttackUnit(OwnerInstance,Target,MyGrid) : nullptr;
	
	FBattleReport Report;
	Report.Attacker = OwnerInstance;
	Report.Defender = Target;
	Report.IsCritical = UBattleFunc::IsCritical(OwnerInstance,Target);;
	Report.Cooperator = Cooperator;
	Report.CooperatorTarget = Cooperator != nullptr ? Target : nullptr;
	Report.IsBackAtk = UBattleFunc::IsBackAttack(OwnerInstance,Target,MyGrid,SkillData.AllowableDeviation);
	Report.HitPercent = UBattleFunc::CalculateHitRate(OwnerInstance,Target,MyGrid,Cooperator != nullptr,Report.IsBackAtk);

	const int Num = FMath::RandRange(0,100);
	if(Num > Report.HitPercent)
	{//未命中
		Report.Damage = 0;
		Report.IsHit = false;
		return MoveTemp(Report);	
	}

	Report.IsHit = true;
	// float atk = OwnerInstance->GetRuntimeProperty().Power;
	// float def = Target->GetRuntimeProperty().PhysicDefend;
	// float per = FMath::FRandRange(0.8,1.0f);
	Report.Damage = UBattleFunc::CalculateDamage(OwnerInstance,Target,MyGrid,10,Report.IsCritical,Report.IsBackAtk);//FMathf::Clamp(atk * per - def,0,999999999);
	//扣除血量
	Target->AddHP(0-Report.Damage);
	
	// ReportList.Add(Report);
	return MoveTemp(Report);
}

