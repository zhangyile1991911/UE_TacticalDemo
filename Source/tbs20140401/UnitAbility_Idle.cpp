// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAbility_Idle.h"
#include "BattleReport.h"

// Sets default values
AUnitAbility_Idle::AUnitAbility_Idle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
}

void AUnitAbility_Idle::BeginDestroy()
{
	Super::BeginDestroy();
}

FBattleReport AUnitAbility_Idle::DoCalculation(const TArray<TObjectPtr<AMyUnit>>& Targets, AGrid* MyGrid,bool NeedCooperator)
{
	return DoCalculation(nullptr,MyGrid,NeedCooperator);
}

FBattleReport AUnitAbility_Idle::DoCalculation(TObjectPtr<AMyUnit> Target, AGrid* MyGrid,bool NeedCooperator)
{
	FBattleReport Report;
	Report.Attacker = OwnerInstance;
	// Report.Defender = nullptr;
	Report.Cooperator = nullptr;
	Report.IsBackAtk = false;
	Report.IsHit = false;
	Report.IsCritical = false;
	Report.HitPercent = 0;
	Report.RandSeed = 0;
	Report.Damage = 0;
	
	return MoveTemp(Report);
}

// Called when the game starts or when spawned
// void AUnitAbility_Idle::BeginPlay()
// {
// 	Super::BeginPlay();
// 	
// }

// Called every frame
// void AUnitAbility_Idle::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// }

