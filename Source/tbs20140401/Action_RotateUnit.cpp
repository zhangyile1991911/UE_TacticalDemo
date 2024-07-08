// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_RotateUnit.h"

#include "Grid.h"
#include "MyUnit.h"
#include "My_Pawn.h"


// Sets default values
AAction_RotateUnit::AAction_RotateUnit()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AAction_RotateUnit::ExecuteAction(const FIntPoint& index)
{
	const FTileData* TileDataPtr = MyPlayerPawn->GetMyGrid()->GetTileDataByIndex(index);

	if(TileDataPtr == nullptr)return;

	if(TileDataPtr->UnitOnTile == nullptr)return;

	
	EUnitDirectType UnitDirect = TileDataPtr->UnitOnTile->GetUnitDirect();
	switch (UnitDirect)
	{
	case EUnitDirectType::LEFT:
		TileDataPtr->UnitOnTile->TurnForward();
		break;
	case EUnitDirectType::RIGHT:
		TileDataPtr->UnitOnTile->TurnBack();
		break;
	case EUnitDirectType::FORWARD:
		TileDataPtr->UnitOnTile->TurnRight();
		break;
	case EUnitDirectType::BACKWARD:
		TileDataPtr->UnitOnTile->TurnLeft();
		break;
	}
	
}
