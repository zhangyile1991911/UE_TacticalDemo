// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAbility_Archery.h"

#include "BattleFunc.h"
#include "BattleReport.h"
#include "Grid.h"
#include "MyUnit.h"
#include "TileData.h"


// Sets default values
AUnitAbility_Archery::AUnitAbility_Archery()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AUnitAbility_Archery::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnitAbility_Archery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AUnitAbility_Archery::CanExecute()
{
	return true;
}

TArray<FIntPoint> AUnitAbility_Archery::Range(const FIntPoint& CenterPoint)
{
	// FIntPoint StandPoint;
	// if(OwnerInstance->NeedToMove())
	// {
	// 	StandPoint = OwnerInstance->GetTempDestinationGridIndex();
	// }
	// else
	// {
	// 	StandPoint  = OwnerInstance->GetGridIndex();
	// }
	// TArray<FIntPoint> IndicatorRange;
	// IndicatorRange.Reserve(16);
	// for(int i = SkillData.Range.X;i <= SkillData.Range.Y;i++)
	// {
	// 	IndicatorRange.Add(FIntPoint(StandPoint.X+i,StandPoint.Y));
	// 	IndicatorRange.Add(FIntPoint(StandPoint.X-i,StandPoint.Y));
	// 	IndicatorRange.Add(FIntPoint(StandPoint.X,StandPoint.Y+i));
	// 	IndicatorRange.Add(FIntPoint(StandPoint.X,StandPoint.Y-i));
	// }
	// return MoveTemp(IndicatorRange);
	TArray<FIntPoint> Path;
	TArray<FIntPoint> Result;
	TSet<FIntPoint> Discovered;
	Path.Reserve(32);
	Result.Reserve(32);
	Discovered.Reserve(32);
	
	auto DiscoverAround = [&Path,&Result,&Discovered,this,CenterPoint](const FIntPoint& Index)
	{
		int DeltaX = 0;
		int DeltaY = 0;
		int Dist = 0;
		FIntPoint UP(Index.X+1,Index.Y);
		DeltaX = FMathf::Abs(UP.X - CenterPoint.X);
		DeltaY = FMathf::Abs(UP.Y - CenterPoint.Y);
		Dist = DeltaX + DeltaY;
		if(Dist <= SkillData.Range.Y)
		{
			if(!Discovered.Contains(UP))
			{
				Path.Add(UP);
				Discovered.Add(UP);
				if(Dist >= SkillData.Range.X)
					Result.Add(UP);
			}
		}

		FIntPoint DOWN(Index.X-1,Index.Y);
		DeltaX = FMathf::Abs(DOWN.X - CenterPoint.X);
		DeltaY = FMathf::Abs(DOWN.Y - CenterPoint.Y);
		Dist = DeltaX + DeltaY;
		if(Dist  <= SkillData.Range.Y)
		{
			if(!Discovered.Contains(DOWN))
			{
				Path.Add(DOWN);
				Discovered.Add(DOWN);
				if(Dist >= SkillData.Range.X)
					Result.Add(DOWN);
			}	
		}

		FIntPoint LEFT(Index.X,Index.Y-1);
		DeltaX = FMathf::Abs(LEFT.X - CenterPoint.X);
		DeltaY = FMathf::Abs(LEFT.Y - CenterPoint.Y);
		Dist = DeltaX + DeltaY;
		if(Dist  <= SkillData.Range.Y)
		{
			if(!Discovered.Contains(LEFT))
			{
				Path.Add(LEFT);
				Discovered.Add(LEFT);
				if(Dist >= SkillData.Range.X)
					Result.Add(LEFT);
			}	
		}

		FIntPoint RIGHT(Index.X,Index.Y+1);
		DeltaX = FMathf::Abs(RIGHT.X - CenterPoint.X);
		DeltaY = FMathf::Abs(RIGHT.Y - CenterPoint.Y);
		Dist = DeltaX + DeltaY;
		if(Dist  <= SkillData.Range.Y)
		{
			if(!Discovered.Contains(RIGHT))
			{
				Discovered.Add(RIGHT);
				Path.Add(RIGHT);
				if(Dist >= SkillData.Range.X)
					Result.Add(RIGHT);
			}
		}
	};
	int PathIndex = 0;
	Path.Add(CenterPoint);
	while(PathIndex < Path.Num())
	{
		const FIntPoint& CurDiscover = Path[PathIndex];
		DiscoverAround(CurDiscover);
		PathIndex++;
	}
	return MoveTemp(Result);
}

bool AUnitAbility_Archery::IsValidTarget(const FTileData* TileData, AGrid* MyGrid)
{
	return true;
}

bool AUnitAbility_Archery::IsValidUnit(TObjectPtr<AMyUnit> Unit)
{
	if(Unit == nullptr)return false;
	if(Unit->IsDead())return false;
	if(Unit->IsFriend(Unit->GetUnitSide()))return false;
	return true;
}

TArray<FIntPoint> AUnitAbility_Archery::Indicator(const FIntPoint& Index)
{
	TArray<FIntPoint> IndicatorRange;
	IndicatorRange.Add(Index);
	return MoveTemp(IndicatorRange);
}

TArray<TObjectPtr<AMyUnit>> AUnitAbility_Archery::TakeTargets(const FIntPoint& Point, AGrid* MyGrid)
{
	const auto TileDataPtr = MyGrid->GetTileDataByIndex(Point);
	TArray<TObjectPtr<AMyUnit>> Targets;
	Targets.Add(TileDataPtr->UnitOnTile);
	return Targets;
}

FBattleReport AUnitAbility_Archery::DoCalculation(const TArray<TObjectPtr<AMyUnit>>& Targets, AGrid* MyGrid,
	bool NeedCooperator)
{
	return DoCalculation(Targets[0], MyGrid, NeedCooperator);
}

FBattleReport AUnitAbility_Archery::DoCalculation(TObjectPtr<AMyUnit> Target, AGrid* MyGrid, bool NeedCooperator)
{
	FBattleReport Report;
	TObjectPtr<AMyUnit> Cooperator = NeedCooperator ? UBattleFunc::HasWrapAttackUnit(OwnerInstance,Target,MyGrid) : nullptr;
	
	Report.Attacker = OwnerInstance;
	Report.Defender = Target;
	Report.IsCritical = UBattleFunc::IsCritical(OwnerInstance,Target);;
	Report.Cooperator = Cooperator;
	Report.CooperatorTarget = Cooperator != nullptr ? Target : nullptr;
	Report.IsBackAtk = false;
	Report.HitPercent = UBattleFunc::CalculateHitRate(OwnerInstance,Target,MyGrid,Cooperator != nullptr,Report.IsBackAtk);

	const int Num = FMath::RandRange(0,100);
	if(Num > Report.HitPercent)
	{//未命中
		Report.Damage = 0;
		Report.IsHit = false;
		return MoveTemp(Report);	
	}

	Report.IsHit = true;
	Report.Damage = 10;
	//扣除血量
	Target->AddHP(Report.Damage);
	
	return MoveTemp(Report);
}

