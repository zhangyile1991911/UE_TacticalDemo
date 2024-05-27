// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAbility_Idle.h"

bool UUnitAbility_Idle::CanExecute()
{
	return true;
}

bool UUnitAbility_Idle::IsValidTarget(const FTileData& TileData)
{
	return true;
}


void UUnitAbility_Idle::Execute()
{
	
}