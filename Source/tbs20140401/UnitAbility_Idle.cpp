// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAbility_Idle.h"


// Sets default values
UUnitAbility_Idle::UUnitAbility_Idle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
}

void UUnitAbility_Idle::BeginDestroy()
{
	Super::BeginDestroy();
}

FBattleReport UUnitAbility_Idle::DoCalculation(TObjectPtr<AMyUnit> Target, AGrid* MyGrid)
{
	FBattleReport Report;
	Report.Attacker = OwnerInstance;
	Report.Defender = nullptr;
	Report.Cooperator = nullptr;
	Report.IsBackAtk = false;
	Report.IsHit = false;
	Report.IsCritical = false;
	Report.HitPercent = 0;
	Report.RandSeed = 0;
	Report.Damage = 0;
	return Report;
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

