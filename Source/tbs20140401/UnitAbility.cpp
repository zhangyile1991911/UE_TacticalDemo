// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAbility.h"

// void AUnitAbility::BeginPlay()
// {
// 	AActor::BeginPlay();
// 	UE_LOG(LogTemp,Log,TEXT("AUnitAbility::BeginPlay()"))
// }

void UUnitAbility::BeginDestroy()
{
	UE_LOG(LogTemp,Log,TEXT("AUnitAbility::BeginDestroy()"))
	UObject::BeginDestroy();
}

// void UUnitAbility::OnAbilityCalculationCompleted(const TArray<FBattleReport>& Reports)
// {
// 	
// }

FBattleReport UUnitAbility::DoCalculation(const TArray<TObjectPtr<AMyUnit>>& Targets, AGrid* MyGrid)
{
	return FBattleReport();
}

FBattleReport UUnitAbility::DoCalculation(TObjectPtr<AMyUnit> Target,AGrid* MyGrid)
{
	return FBattleReport();
}

TArray<TObjectPtr<AMyUnit>> UUnitAbility::TakeTargets(const FIntPoint& Point, AGrid* MyGrid)
{
	TArray<TObjectPtr<AMyUnit>> Targets;
	return MoveTemp(Targets);
}

// void UUnitAbility::OnAbilityCompleted()
// {
// 	if(CompletedEvent.IsBound())
// 	{
// 		CompletedEvent.Broadcast(this);
// 	}
// 	if(CompletedCallback.IsBound())
// 	{
// 		CompletedCallback.Execute(this);
// 	}
// }
