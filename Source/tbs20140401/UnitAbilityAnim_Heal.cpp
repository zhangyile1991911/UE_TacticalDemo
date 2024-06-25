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


TArray<FIntPoint> AUnitAbilityAnim_Heal::Range(const FIntPoint& CenterPoint)
{
	TArray<FIntPoint> Result;
	TSet<FIntPoint> Discovered;
	Result.Reserve(32);
	Discovered.Reserve(32);
	
	auto DiscoverAround = [&Result,&Discovered,this,CenterPoint](const FIntPoint& Index)
	{
		int DeltaX = 0;
		int DeltaY = 0;

		FIntPoint UP(Index.X+1,Index.Y);
		DeltaX = FMathf::Abs(UP.X - CenterPoint.X);
		DeltaY = FMathf::Abs(UP.Y - CenterPoint.Y);
		if(DeltaX+DeltaY <= SkillData.Range.Y)
		{
			if(!Discovered.Contains(UP))
			{
				Result.Add(UP);
				Discovered.Add(UP);
			}
		}

		FIntPoint DOWN(Index.X-1,Index.Y);
		DeltaX = FMathf::Abs(DOWN.X - CenterPoint.X);
		DeltaY = FMathf::Abs(DOWN.Y - CenterPoint.Y);
		if(DeltaX+DeltaY <= SkillData.Range.Y)
		{
			if(!Discovered.Contains(DOWN))
			{
				Result.Add(DOWN);
				Discovered.Add(DOWN);
			}	
		}

		FIntPoint LEFT(Index.X,Index.Y-1);
		DeltaX = FMathf::Abs(LEFT.X - CenterPoint.X);
		DeltaY = FMathf::Abs(LEFT.Y - CenterPoint.Y);
		if(DeltaX+DeltaY <= SkillData.Range.Y)
		{
			if(!Discovered.Contains(LEFT))
			{
				Result.Add(LEFT);
				Discovered.Add(LEFT);
			}	
		}

		FIntPoint RIGHT(Index.X,Index.Y+1);
		DeltaX = FMathf::Abs(RIGHT.X - CenterPoint.X);
		DeltaY = FMathf::Abs(RIGHT.Y - CenterPoint.Y);
		if(DeltaX+DeltaY <= SkillData.Range.Y)
		{
			if(!Discovered.Contains(RIGHT))
			{
				Discovered.Add(RIGHT);
				Result.Add(RIGHT);
			}
		}
	};
	int PathIndex = 0;
	Result.Add(CenterPoint);
	while(PathIndex < Result.Num())
	{
		const FIntPoint& CurDiscover = Result[PathIndex];
		DiscoverAround(CurDiscover);
		PathIndex++;
	}
	
	return MoveTemp(Result);
}

TArray<FIntPoint> AUnitAbilityAnim_Heal::Indicator(const FIntPoint& Index)
{
	TArray<FIntPoint> Result;
	Result.Add(Index);
	return MoveTemp(Result);
}

bool AUnitAbilityAnim_Heal::IsValidTarget(const FTileData& TileData, AGrid* MyGrid)
{
	if(TileData.UnitOnTile == nullptr)return false;
	if(TileData.UnitOnTile->IsDead())return false;
	if(TileData.UnitOnTile->IsFriend(OwnerInstance->GetUnitSide()))return true;
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
	return DoCalculation(Targets[0], MyGrid, NeedCooperator);
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

