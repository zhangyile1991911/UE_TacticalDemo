// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_Move.h"

#include "MyCombatSystem.h"
#include "MyGridPathfinding.h"
#include "My_Pawn.h"
#include "MyUnit.h"
#include "MyGridPathfinding.h"

void UPawnProcess_Move::PathFindingCompleted(TArray<FIntPoint> Path)
{
	FPathCompleted completed;
	completed.BindUObject(this,&UPawnProcess_Move::UnitMoveToTarget);
	UnitInstance->SetWalkPath(MoveTemp(Path),completed);
}

void UPawnProcess_Move::UnitMoveToTarget()
{
	UnitInstance->WalkDone();
	PawnInstance->SwitchToCalcAnim();
}

void UPawnProcess_Move::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	Super::EnterProcess(Pawn);

	UnitInstance = PawnInstance->GetMyCombatSystem()->GetFirstUnit();
	FIntPoint StartLocation = UnitInstance->GetGridIndex();
	FIntPoint FinishLocation = UnitInstance->GetTempDestination();
	FPathFindingCompleted Completed;
	Completed.BindUObject(this,&UPawnProcess_Move::PathFindingCompleted);
	PawnInstance->GetMyGridPathFinding()->UnitFindPath(StartLocation,FinishLocation,UnitInstance->UnitCanWalkTileType(),Completed);
}

void UPawnProcess_Move::TickProcess()
{
	Super::TickProcess();
	
}

void UPawnProcess_Move::HandleDirectionInput(const FVector2D& Input)
{
	Super::HandleDirectionInput(Input);
	
}

void UPawnProcess_Move::HandleCancelInput()
{
	Super::HandleCancelInput();
	
}

void UPawnProcess_Move::HandleConfirmInput()
{
	Super::HandleConfirmInput();
	
}

void UPawnProcess_Move::ExitProcess()
{
	Super::ExitProcess();
	
}
