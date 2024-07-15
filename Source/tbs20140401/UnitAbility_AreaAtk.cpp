// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAbility_AreaAtk.h"

#include "BattleFunc.h"
#include "BattleReport.h"
#include "Grid.h"
#include "MyGridPathfinding.h"
#include "MyUnit.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AUnitAbility_AreaAtk::AUnitAbility_AreaAtk()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AUnitAbility_AreaAtk::BeginPlay()
{
	Super::BeginPlay();
	// AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(),AMyGridPathfinding::StaticClass());
	// MyGridPathfinding = Cast<AMyGridPathfinding>(Actor);
	
}

void AUnitAbility_AreaAtk::BeginDestroy()
{
	Super::BeginDestroy();
	// MyGridPathfinding = nullptr;
}

bool AUnitAbility_AreaAtk::CanExecute()
{
	return OwnerInstance->HasEnoughAP(SkillData.SpendPoint);
}

TArray<FIntPoint> AUnitAbility_AreaAtk::Range(const FIntPoint& Point)
{
	// return MyGridPathfinding->UnitAbilityRange(Point,SkillData.Range);
	TArray<FIntPoint> ArrayOfRange;
	ArrayOfRange.Reserve(32);
	TSet<FIntPoint> Discovered;
	Discovered.Reserve(32);

	ArrayOfRange.Add(Point);
	Discovered.Add(Point);
	int ReachIndex = 0;
	while(ReachIndex < ArrayOfRange.Num())
	{
		const FIntPoint& CurrentPoint = ArrayOfRange[ReachIndex];
		FIntPoint UP(CurrentPoint.X+1,CurrentPoint.Y);
		FIntPoint RIGHT(CurrentPoint.X,CurrentPoint.Y+1);
		FIntPoint DOWN(CurrentPoint.X-1,CurrentPoint.Y);
		FIntPoint LEFT(CurrentPoint.X,CurrentPoint.Y-1);
		int Dist = FMathf::Abs(UP.X - Point.X) + FMathf::Abs(UP.Y - Point.Y);
		if(Discovered.Contains(UP) == false && Dist <= SkillData.Range.Y)
		{
			Discovered.Add(UP);
			ArrayOfRange.Add(UP);
		}
		Dist = FMathf::Abs(RIGHT.X - Point.X) + FMathf::Abs(RIGHT.Y - Point.Y);
		if(Discovered.Contains(RIGHT) == false && Dist <= SkillData.Range.Y)
		{
			Discovered.Add(RIGHT);
			ArrayOfRange.Add(RIGHT);
		}
		Dist = FMathf::Abs(DOWN.X - Point.X) + FMathf::Abs(DOWN.Y - Point.Y);
		if(Discovered.Contains(DOWN) == false && Dist <= SkillData.Range.Y)
		{
			Discovered.Add(DOWN);
			ArrayOfRange.Add(DOWN);
		}
		Dist = FMathf::Abs(LEFT.X - Point.X) + FMathf::Abs(LEFT.Y - Point.Y);
		if(Discovered.Contains(LEFT) == false && Dist <= SkillData.Range.Y)
		{
			Discovered.Add(LEFT);
			ArrayOfRange.Add(LEFT);
		}
		ReachIndex += 1;
	}
	
	return MoveTemp(ArrayOfRange);
}

TArray<FIntPoint> AUnitAbility_AreaAtk::Indicator(const FIntPoint& Index)
{
	TArray<FIntPoint> Area;
	Area.Reserve(5);
	Area.Add(FIntPoint(Index.X,Index.Y));
	Area.Add(FIntPoint(Index.X+1,Index.Y));
	Area.Add(FIntPoint(Index.X-1,Index.Y));
	Area.Add(FIntPoint(Index.X,Index.Y+1));
	Area.Add(FIntPoint(Index.X,Index.Y-1));
	return Area;
}

bool AUnitAbility_AreaAtk::IsValidTarget(const FTileData* TileData,AGrid* MyGrid)
{
	if(TileData == nullptr)return false;
	//スキルの範囲ないで、敵がなかったら　実行できない
	
	const FIntPoint& StandIndex = OwnerInstance->GetStandGridIndex();
	const FTileData* StandTileData = MyGrid->GetTileDataByIndex(StandIndex);
	const bool bIsDeviation = CheckDeviation(TileData->Height,StandTileData->Height);
	return bIsDeviation;
}

bool AUnitAbility_AreaAtk::IsValidUnit(TObjectPtr<AMyUnit> Unit)
{
	return true;
}

TArray<TObjectPtr<AMyUnit>> AUnitAbility_AreaAtk::TakeTargets(const FIntPoint& Point, AGrid* MyGrid)
{
	TArray<TObjectPtr<AMyUnit>> Targets;
	Targets.Reserve(5);
	TArray<FIntPoint> Area;
	Area.Reserve(5);
	Area.Add(Point);
	Area.Add(FIntPoint(Point.X+1,Point.Y));
	Area.Add(FIntPoint(Point.X-1,Point.Y));
	Area.Add(FIntPoint(Point.X,Point.Y+1));
	Area.Add(FIntPoint(Point.X,Point.Y-1));

	const FIntPoint& StandIndex = OwnerInstance->GetStandGridIndex();
	const FTileData* StandTileData = MyGrid->GetTileDataByIndex(StandIndex);
	
	for(const auto& one : Area)
	{
		TObjectPtr<AMyUnit> TargetUnit = MyGrid->GetUnitOnTile(one);
		if(TargetUnit == nullptr)continue;

		bool IsFriend = TargetUnit->IsFriend(OwnerInstance->GetUnitSide());
		if(IsFriend)continue;

		const auto TileData = MyGrid->GetTileDataByIndex(one);
		const bool bIsDeviation = CheckDeviation(TileData->Height,StandTileData->Height);
		if(bIsDeviation)
			Targets.Add(TargetUnit);
	}
	return MoveTemp(Targets);
}

FBattleReport AUnitAbility_AreaAtk::DoCalculation(const TArray<TObjectPtr<AMyUnit>>& Targets, AGrid* MyGrid,
	bool NeedCooperator)
{
	FBattleReport Result;
	Result.Attacker = OwnerInstance;
	
	for(int i = 0;i < Targets.Num();i++)
	{
		FHitInfo HitInfo;
		auto OneTarget = Targets[i];
		HitInfo.Defender = OneTarget;
		HitInfo.IsCritical = UBattleFunc::IsCritical(OwnerInstance,OneTarget);
		HitInfo.Cooperator = nullptr;
		
		HitInfo.IsHit = true;
		float atk = OwnerInstance->GetRuntimeProperty().Power;
		float def = OneTarget->GetRuntimeProperty().PhysicDefend;
		float per = FMath::FRandRange(0.8,1.0f);
		HitInfo.Damage = UBattleFunc::CalculateDamage(OwnerInstance,Targets[i],MyGrid,10,HitInfo.IsCritical,false);
		OneTarget->AddHP(0-HitInfo.Damage);
		
		Result.HitInfoList.Add(HitInfo);
	}
	
	return MoveTemp(Result);
}

FBattleReport AUnitAbility_AreaAtk::DoCalculation(TObjectPtr<AMyUnit> Target, AGrid* MyGrid, bool NeedCooperator)
{
	return Super::DoCalculation(Target, MyGrid, NeedCooperator);
}
