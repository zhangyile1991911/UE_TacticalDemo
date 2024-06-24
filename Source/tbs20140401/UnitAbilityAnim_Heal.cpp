// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAbilityAnim_Heal.h"

#include "BattleReport.h"
#include "MyUnit.h"
#include "Grid.h"

// Sets default values
AUnitAbilityAnim_Heal::AUnitAbilityAnim_Heal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AUnitAbilityAnim_Heal::BeginPlay()
{
	Super::BeginPlay();
}

bool AUnitAbilityAnim_Heal::CanExecute()
{
	return true;
}

TArray<FIntPoint> AUnitAbilityAnim_Heal::Range(const FIntPoint& Int32Point)
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

bool AUnitAbilityAnim_Heal::IsValidTarget(const FTileData& TileData, AGrid* MyGrid)
{
	auto IndicatorRange = Indicator(TileData.Index);
	for(const auto& One : IndicatorRange)
	{
		const auto TempPtr = MyGrid->GetTileDataByIndex(One);
		if(TempPtr == nullptr)continue;
		if(TempPtr->UnitOnTile == nullptr)continue;
		if(TempPtr->UnitOnTile->IsDead())continue;
		if(TempPtr->UnitOnTile->IsFriend(OwnerInstance->GetUnitSide()))continue;

		return true;
	}
	
	return false;
}

bool AUnitAbilityAnim_Heal::IsValidUnit(TObjectPtr<AMyUnit> Unit)
{
	if(Unit == nullptr)return false;
	if(Unit->IsDead())return false;
	if(!Unit->IsFriend(OwnerInstance->GetUnitSide()))return false;
	return true;
}

TArray<TObjectPtr<AMyUnit>> AUnitAbilityAnim_Heal::TakeTargets(const FIntPoint& Point, AGrid* MyGrid)
{
	TArray<TObjectPtr<AMyUnit>> Targets;
	const FTileData* TileDataPtr = MyGrid->GetTileDataByIndex(Point);

	if(TileDataPtr == nullptr)return Targets;
	if(TileDataPtr->UnitOnTile == nullptr)return Targets;
	if(!TileDataPtr->UnitOnTile->IsFriend(OwnerInstance->GetUnitSide()))return Targets;
	Targets.Add(TileDataPtr->UnitOnTile);
	
	return MoveTemp(Targets);
}

FBattleReport AUnitAbilityAnim_Heal::DoCalculation(const TArray<TObjectPtr<AMyUnit>>& Targets, AGrid* MyGrid,
	bool NeedCooperator)
{
	return Super::DoCalculation(Targets, MyGrid, NeedCooperator);
}

FBattleReport AUnitAbilityAnim_Heal::DoCalculation(TObjectPtr<AMyUnit> Target, AGrid* MyGrid, bool NeedCooperator)
{
	FBattleReport Report;

	Report.Attacker = OwnerInstance;
	Report.Defender = Target;
	Report.IsCritical = false;
	Report.Cooperator = nullptr;
	Report.CooperatorTarget = nullptr;
	Report.IsBackAtk = false;
	Report.HitPercent = 100;

	Report.IsHit = true;
	Report.Damage = -10;
	//扣除血量
	Target->AddHP(Report.Damage);
	
	return MoveTemp(Report);
}

