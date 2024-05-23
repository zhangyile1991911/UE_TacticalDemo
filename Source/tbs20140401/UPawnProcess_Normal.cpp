// Fill out your copyright notice in the Description page of Project Settings.


#include "UPawnProcess_Normal.h"

#include "MyUnit.h"
#include "Grid.h"
#include "My_Pawn.h"

void UUPawnProcess_Normal::EnterProcess(TObjectPtr<AMy_Pawn> Pawn,TObjectPtr<AMyUnit> Unit)
{
	Super::EnterProcess(Pawn,Unit);

	TArray<FIntPoint> WalkableRange = PawnInstance->GetMyGridPathFinding()->UnitWalkablePath(
			UnitInstance->GetGridIndex(),
			UnitInstance->GetProperty().WalkRange,
			UnitInstance->UnitCanWalkTileType());
	UnitInstance->SetWalkableTile(WalkableRange);

	for(const FIntPoint& one : WalkableRange)
	{
		PawnInstance->GetMyGrid()->AddStateToTile(one,ETileState::Reachable);
	}
	
	Completed.BindUObject(this,&UUPawnProcess_Normal::ShowWalkableTiles);

	PreviousPathFinding.Empty();
	PawnInstance->UpdateTIleByIndex(Unit->GetGridIndex(),ETileState::Selected);
	PawnInstance->GetMyGrid()->AddStateToTile(Unit->GetGridIndex(),ETileState::Selected);
}

void UUPawnProcess_Normal::TickProcess()
{
	Super::TickProcess();
}

void UUPawnProcess_Normal::HandleDirectionInput(const FIntPoint& Index)
{
	Super::HandleDirectionInput(Index);
	CurrentCursor = Index;

	ClearPathFinding();
	
	
	if(UnitInstance->IsInWalkableRange(Index))
	{
		PawnInstance->GetMyGridPathFinding()->UnitFindPath(
			UnitInstance->GetGridIndex(),
			Index,
			UnitInstance->UnitCanWalkTileType(),
			Completed);
	}
	else
	{
		
		bool HasUnit = PawnInstance->GetMyGrid()->TileGridHasUnit(Index);
		if(HasUnit)
		{//todo 显示头像
			
		}
		else
		{//todo 隐藏头像
			
		}
	}
	
}

void UUPawnProcess_Normal::HandleCancelInput()
{
	Super::HandleCancelInput();
	UnitInstance->HideShadowUnit();
	
	ClearPathFinding();
	
	PawnInstance->UpdateTIleByIndex(UnitInstance->GetGridIndex(),ETileState::Selected);
	
}

void UUPawnProcess_Normal::HandleConfirmInput()
{
	Super::HandleConfirmInput();
	if(!UnitInstance->IsInWalkableRange(CurrentCursor))
	{
		return;
	}

	//将影子放到目的地上
	//キャラの影を目的に置いとく
	const auto TileData = PawnInstance->GetMyGrid()->GetTileDataByIndex(CurrentCursor);
	if(TileData == nullptr)
	{
		UE_LOG(LogTemp,Log,TEXT("CurrentCursor(%d,%d)"),CurrentCursor.X,CurrentCursor.Y);
		return;
	}
	UnitInstance->MoveShadowOnTile(TileData->Transform.GetLocation());
}

void UUPawnProcess_Normal::ExitProcess()
{
	Super::ExitProcess();
}


void UUPawnProcess_Normal::ShowWalkableTiles(TArray<FIntPoint> tiles)
{
	for(const FIntPoint& one : tiles)
	{
		PawnInstance->GetMyGrid()->AddStateToTile(one,ETileState::PathFinding);
	}
	PreviousPathFinding = MoveTemp(tiles);
	
}

void UUPawnProcess_Normal::ClearPathFinding()
{
	for(const FIntPoint& one : PreviousPathFinding)
	{
		PawnInstance->GetMyGrid()->RemoveStateFromTile(one,ETileState::PathFinding);
	}
	PreviousPathFinding.Empty();
}