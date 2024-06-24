// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAbility_BeatBack.h"

#include "BattleFunc.h"
#include "BattleReport.h"
#include "Grid.h"
#include "MyUnit.h"


// Sets default values
AUnitAbility_BeatBack::AUnitAbility_BeatBack()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AUnitAbility_BeatBack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnitAbility_BeatBack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AUnitAbility_BeatBack::CanExecute()
{
	return true;
}

TArray<FIntPoint> AUnitAbility_BeatBack::Range(const FIntPoint& Int32Point)
{
	TArray<FIntPoint> RangeTiles;
	RangeTiles.Add(FIntPoint(Int32Point.X+1,Int32Point.Y));
	RangeTiles.Add(FIntPoint(Int32Point.X-1,Int32Point.Y));
	RangeTiles.Add(FIntPoint(Int32Point.X,Int32Point.Y+1));
	RangeTiles.Add(FIntPoint(Int32Point.X,Int32Point.Y-1));
	return MoveTemp(RangeTiles);
}

bool AUnitAbility_BeatBack::IsValidTarget(const FTileData& TileData, AGrid* MyGrid)
{
	if(TileData.UnitOnTile == nullptr)return false;
	if(TileData.UnitOnTile->IsDead())return false;
	if(TileData.UnitOnTile->IsFriend(OwnerInstance->GetUnitSide()))return false;
	return true;
}

bool AUnitAbility_BeatBack::IsValidUnit(TObjectPtr<AMyUnit> Unit)
{
	if(Unit == nullptr)return false;
	if(Unit->IsDead())return false;
	if(Unit->IsFriend(OwnerInstance->GetUnitSide()))return false;
	return true;
}

TArray<FIntPoint> AUnitAbility_BeatBack::Indicator(const FIntPoint& Index)
{
	TArray<FIntPoint> IndicatorRange;
	IndicatorRange.Add(Index);
	return MoveTemp(IndicatorRange);
}

TArray<TObjectPtr<AMyUnit>> AUnitAbility_BeatBack::TakeTargets(const FIntPoint& Point, AGrid* MyGrid)
{
	TArray<TObjectPtr<AMyUnit>> Targets;
	const auto TileDataPtr = MyGrid->GetTileDataByIndex(Point);
	if(TileDataPtr == nullptr)return Targets;
	
	Targets.Add(TileDataPtr->UnitOnTile);
	return Targets;
}

FBattleReport AUnitAbility_BeatBack::DoCalculation(const TArray<TObjectPtr<AMyUnit>>& Targets, AGrid* MyGrid,
	bool NeedCooperator)
{
	return DoCalculation(Targets[0], MyGrid, NeedCooperator);
}

FBattleReport AUnitAbility_BeatBack::DoCalculation(TObjectPtr<AMyUnit> Target, AGrid* MyGrid, bool NeedCooperator)
{
	FBattleReport Report;
	Report.Attacker = OwnerInstance;
	Report.Defender = Target;
	Report.IsCritical = UBattleFunc::IsCritical(OwnerInstance,Target);;
	Report.Cooperator = nullptr;
	Report.CooperatorTarget = nullptr;
	Report.IsBackAtk = UBattleFunc::IsBackAttack(OwnerInstance,Target);
	Report.HitPercent = UBattleFunc::CalculateHitRate(OwnerInstance,Target,MyGrid,false,Report.IsBackAtk);

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
	//先确认方向 攻撃者の向いている方向を取得する
	auto EDirectType = OwnerInstance->GetUnitDirect();
	FIntPoint VecIntPoint;
	switch (EDirectType)
	{
	case EUnitDirectType::LEFT:
		VecIntPoint.Y = 1;
		break;
	case EUnitDirectType::RIGHT:
		VecIntPoint.Y = -1;
		break;
	case EUnitDirectType::FORWARD:
		VecIntPoint.X = 1;
		break;
	case EUnitDirectType::BACKWARD:
		VecIntPoint.X = -1;
		break;
	}
	//沿着方向一直推　その方向に沿って押す
	FIntPoint TargetIntPoint = Target->GetGridIndex();
	TargetIntPoint += VecIntPoint;
	while(MyGrid->IsValidGridIndex(TargetIntPoint) && !MyGrid->TileGridHasUnit(TargetIntPoint))
	{
		TargetIntPoint += VecIntPoint;
	}
	TargetIntPoint -= VecIntPoint;
	const FTileData* DestPtr = MyGrid->GetTileDataByIndex(TargetIntPoint);
	Report.FirstIndex = DestPtr->Transform.GetLocation();
	//ターゲットの位置を更新する
	MyGrid->AddTileDataUnitByIndex(Target->GetGridIndex(),nullptr);
	MyGrid->AddTileDataUnitByIndex(TargetIntPoint,Target);
	Target->SetGridIndex(TargetIntPoint);
	
	return MoveTemp(Report);
}

