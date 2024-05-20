// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_FindPath.h"

#include "Grid.h"
#include "MyGridPathfinding.h"
#include "MyUnit.h"
#include "My_Pawn.h"


void AAction_FindPath::BeginPlay()
{
	Super::BeginPlay();
}

void AAction_FindPath::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	MyPlayerPawn->GetMyGrid()->RemoveStateAllTile(ETileState::PathFinding);
}

void AAction_FindPath::ExecuteAction(const FIntPoint& index)
{
	Super::ExecuteAction(index);
	Start = MyPlayerPawn->GetSelectedTile();
	Finish = index;
	//清除上一次寻路的格子状态
	MyPlayerPawn->GetMyGrid()->RemoveStateAllTile(ETileState::PathFinding);

	auto Unit = MyPlayerPawn->GetMyGrid()->GetUnitOnTile(Start);
	if(Unit != nullptr)
	{
		if(Unit->IsInWalkableTile(Finish))
		{
			const FTileData* pData = MyPlayerPawn->GetMyGrid()->GetTileDataByIndex(Start);
			MyPlayerPawn->GetMyGridPathFinding()->UnitFindPath(Start,Finish,pData->UnitOnTile->UnitCanWalkTileType(),FPathFindingCompleted::CreateUObject(this,&AAction_FindPath::WaitPathFinding));	
		}
	}
	else
	{
		MyPlayerPawn->GetMyGridPathFinding()->FindPath(Start,Finish,FPathFindingCompleted::CreateUObject(this,&AAction_FindPath::WaitPathFinding));	
	}
	
	
}

void AAction_FindPath::WaitPathFinding(TArray<FIntPoint> path)
{
	for(const FIntPoint& one : path)
    {
    	MyPlayerPawn->GetMyGrid()->AddStateToTile(one,ETileState::PathFinding);	
    }
	if(MyPlayerPawn->GetMyGrid()->TileGridHasUnit(Start))
	{
		MyPlayerPawn->GetSelectedUnit()->SetWalkPath(MoveTemp(path));
	}
}
