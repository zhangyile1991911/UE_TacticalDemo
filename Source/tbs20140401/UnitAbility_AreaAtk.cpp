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
	AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(),AMyGridPathfinding::StaticClass());
	MyGridPathfinding = Cast<AMyGridPathfinding>(Actor);
	
}

void AUnitAbility_AreaAtk::BeginDestroy()
{
	Super::BeginDestroy();
	MyGridPathfinding = nullptr;
}

bool AUnitAbility_AreaAtk::CanExecute()
{
	return true;
}

TArray<FIntPoint> AUnitAbility_AreaAtk::Range(const FIntPoint& Point)
{
	return MyGridPathfinding->UnitAbilityRange(Point,SkillData.Range);
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

bool AUnitAbility_AreaAtk::IsValidTarget(const FTileData& TileData,AGrid* MyGrid)
{
	//スキルの範囲ないで、敵がなかったら　実行できない
	FIntPoint Index = TileData.Index;
	TArray<FIntPoint> Area;
	Area.Reserve(5);
	Area.Add(FIntPoint(Index.X,Index.Y));
	Area.Add(FIntPoint(Index.X+1,Index.Y));
	Area.Add(FIntPoint(Index.X-1,Index.Y));
	Area.Add(FIntPoint(Index.X,Index.Y+1));
	Area.Add(FIntPoint(Index.X,Index.Y-1));

	for(int i = 0;i < Area.Num();i++)
	{
		const FTileData* Tmp = MyGrid->GetTileDataByIndex(Area[i]);
		if(Tmp->UnitOnTile == nullptr)continue;
		if(!Tmp->UnitOnTile->IsFriend(OwnerInstance->GetUnitSide()))return true;
	}
	
	return false;
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
	for(const auto& one : Area)
	{
		TObjectPtr<AMyUnit> TargetUnit = MyGrid->GetUnitOnTile(one);
		if(TargetUnit == nullptr)continue;

		bool IsFriend = TargetUnit->IsFriend(OwnerInstance->GetUnitSide());
		if(IsFriend)continue;

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
		HitInfo.Damage = 10;
		OneTarget->AddHP(HitInfo.Damage);
		
		Result.HitInfoList.Add(HitInfo);
	}
	
	return MoveTemp(Result);
}

FBattleReport AUnitAbility_AreaAtk::DoCalculation(TObjectPtr<AMyUnit> Target, AGrid* MyGrid, bool NeedCooperator)
{
	return Super::DoCalculation(Target, MyGrid, NeedCooperator);
}
