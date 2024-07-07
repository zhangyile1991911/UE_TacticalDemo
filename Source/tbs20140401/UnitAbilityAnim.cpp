// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAbilityAnim.h"
#include "BattleReport.h"
#include "Grid.h"
#include "MyGridPathfinding.h"
#include "MyUnit.h"

// Sets default values
AUnitAbilityAnim::AUnitAbilityAnim()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AUnitAbilityAnim::BeginPlay()
{
	Super::BeginPlay();
}

bool AUnitAbilityAnim::CheckDeviation(int HeightA,int HeightB)
{
	const int HeightDelta = FMath::Abs(HeightA - HeightB);
	if(HeightDelta > SkillData.AllowableDeviation)return false;
	return true;
}

bool AUnitAbilityAnim::CheckTileDataHeight(const FTileData* TileDataPtr, AGrid* MyGrid)
{
	const FIntPoint& StandIndex = OwnerInstance->GetStandGridIndex();
	const FTileData* StandTileData = MyGrid->GetTileDataByIndex(StandIndex);
	return CheckDeviation(TileDataPtr->Height,StandTileData->Height);
}

// Called every frame
void AUnitAbilityAnim::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AUnitAbilityAnim::OnAbilityCompleted()
{
	if(CompletedEvent.IsBound())
	{
		CompletedEvent.Broadcast(this);
	}
	if(CompletedCallback.IsBound())
	{
		CompletedCallback.Execute(this);
	}
}

bool AUnitAbilityAnim::IsValidUnit(TObjectPtr<AMyUnit> Unit)
{
	return false;
}

FBattleReport AUnitAbilityAnim::DoCalculation(const TArray<TObjectPtr<AMyUnit>>& Targets, AGrid* MyGrid,bool NeedCooperator)
{
	return FBattleReport();
}

FBattleReport AUnitAbilityAnim::DoCalculation(TObjectPtr<AMyUnit> Target,AGrid* MyGrid,bool NeedCooperator)
{
	return FBattleReport();
}

TArray<TObjectPtr<AMyUnit>> AUnitAbilityAnim::TakeTargets(const FIntPoint& Point, AGrid* MyGrid)
{
	TArray<TObjectPtr<AMyUnit>> Targets;
	return MoveTemp(Targets);
}


