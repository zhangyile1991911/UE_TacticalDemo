// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_SelectTile.h"

#include "Grid.h"
#include "GridShapeData.h"
#include "MyGridPathfinding.h"
#include "MyUnit.h"
#include "My_Pawn.h"


void AAction_SelectTile::ExecuteAction(const FIntPoint& index)
{
	// MyPlayerPawn->RemoveTileStateByIndex(selectedIndex,ETileState::Selected);
	MyPlayerPawn->UpdateTIleByIndex(index,ETileState::Selected);
	selectedIndex = index;

	auto Unit = MyPlayerPawn->GetMyGrid()->GetUnitOnTile(index);
	if(SelectedUnit == Unit)
	{
		return;
	}
	SelectedUnit = Unit;
	MyPlayerPawn->GetMyGrid()->RemoveStateAllTile(ETileState::Reachable);
	if(Unit)
	{
		TArray<FIntPoint> WalkableTiles = MyPlayerPawn->GetMyGridPathFinding()->UnitWalkablePath(index,5,Unit->UnitCanWalkTileType());
		for(auto& one : WalkableTiles)
		{
			MyPlayerPawn->GetMyGrid()->AddStateToTile(one,ETileState::Reachable);	
		}
		SelectedUnit->SetWalkableTile(MoveTemp(WalkableTiles));
	}
	
}

void AAction_SelectTile::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp,Log,TEXT("AAction_SelectTile::BeginDestroy()"))

	if(MyPlayerPawn)MyPlayerPawn->RemoveTileStateByIndex(selectedIndex,ETileState::Selected);
}

void AAction_SelectTile::Destroyed()
{
	Super::Destroyed();
	UE_LOG(LogTemp,Log,TEXT("AAction_SelectTile::Destroyed()"))
}




