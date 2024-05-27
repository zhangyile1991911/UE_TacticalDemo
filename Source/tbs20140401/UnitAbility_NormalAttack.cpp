// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAbility_NormalAttack.h"

#include "MyUnit.h"
#include  "TileData.h"
bool UUnitAbility_NormalAttack::CanExecute()
{
	return true;
}

bool UUnitAbility_NormalAttack::IsValidTarget(const FTileData& TileData)
{
	if(TileData.UnitOnTile == nullptr)return false;

	if(TileData.UnitOnTile->GetRuntimeProperty().UnitSide == OwnerInstance->GetRuntimeProperty().UnitSide)return false;
	
	return true;
}

void UUnitAbility_NormalAttack::Execute()
{
	Super::Execute();
}

TArray<FIntPoint> UUnitAbility_NormalAttack::Range(const FIntPoint& Center)
{
	TArray<FIntPoint> Result;
	Result.Add(FIntPoint(Center.X+1,Center.Y));
	Result.Add(FIntPoint(Center.X-1,Center.Y));
	Result.Add(FIntPoint(Center.X,Center.Y+1));
	Result.Add(FIntPoint(Center.X,Center.Y-1));
	
	return Result;
}
