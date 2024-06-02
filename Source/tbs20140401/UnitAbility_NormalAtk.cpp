// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAbility_NormalAtk.h"
#include "BattleFunc.h"
#include "MyUnit.h"
#include "Grid.h"


// Sets default values
UUnitAbility_NormalAtk::UUnitAbility_NormalAtk()
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

bool UUnitAbility_NormalAtk::CanExecute()
{
	return true;
}

TArray<FIntPoint> UUnitAbility_NormalAtk::Range(const FIntPoint& Int32Point)
{
	TArray<FIntPoint> Result;
	Result.Add(FIntPoint(Int32Point.X + 1,Int32Point.Y));
	Result.Add(FIntPoint(Int32Point.X - 1,Int32Point.Y));
	Result.Add(FIntPoint(Int32Point.X,Int32Point.Y + 1));
	Result.Add(FIntPoint(Int32Point.X,Int32Point.Y - 1));
	return MoveTemp(Result);
}

bool UUnitAbility_NormalAtk::IsValidTarget(const FTileData& TileData)
{
	return TileData.UnitOnTile == nullptr ?
		false :
		TileData.UnitOnTile->GetRuntimeProperty().UnitSide != OwnerInstance->GetRuntimeProperty().UnitSide;
}

TArray<TObjectPtr<AMyUnit>> UUnitAbility_NormalAtk::TakeTargets(const FIntPoint& Point, AGrid* MyGrid)
{
	TArray<TObjectPtr<AMyUnit>> Targets;
	TObjectPtr<AMyUnit> one = MyGrid->GetUnitOnTile(Point);
	if(one != nullptr)Targets.Add(one);
	return MoveTemp(Targets);
}

FBattleReport UUnitAbility_NormalAtk::DoCalculation(const TArray<TObjectPtr<AMyUnit>>& Targets, AGrid* MyGrid)
{
	Super::DoCalculation(Targets, MyGrid);
	
	return DoCalculation(Targets[0],MyGrid);
}

FBattleReport UUnitAbility_NormalAtk::DoCalculation(TObjectPtr<AMyUnit> Target, AGrid* MyGrid)
{
	Super::DoCalculation(Target, MyGrid);
	
	TObjectPtr<AMyUnit> Cooperator = UBattleFunc::GetWrapAttackUnit(OwnerInstance,Target,MyGrid);
	
	FBattleReport Report;
	Report.Attacker = OwnerInstance;
	Report.Defender = Target;
	Report.IsCritical = UBattleFunc::IsCritical(OwnerInstance,Target);;
	Report.Cooperator = Cooperator;
	Report.IsBackAtk = UBattleFunc::IsBackAttack(OwnerInstance,Target);
	Report.HitPercent = UBattleFunc::CalculateHitRate(OwnerInstance,Target,MyGrid,Cooperator != nullptr,Report.IsBackAtk);

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
	Report.Damage = FMathf::Clamp(atk * per - def,0,999999999);
	return MoveTemp(Report);
}

