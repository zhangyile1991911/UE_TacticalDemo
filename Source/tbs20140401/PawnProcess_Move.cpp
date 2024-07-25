// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_Move.h"

#include "Grid.h"
#include "MyCombatSystem.h"
#include "My_Pawn.h"
#include "MyUnit.h"

// void UPawnProcess_Move::PathFindingCompleted(TArray<FIntPoint> Path)
// {
// 	FPathCompleted completed;
// 	completed.BindUObject(this,&UPawnProcess_Move::UnitMoveToTarget);
// 	UnitInstance->SetWalkPath(MoveTemp(Path),completed);
// }

void UPawnProcess_Move::UnitMoveToTarget()
{
	UnitInstance->WalkDone();
	// UnitInstance->RotateSelfByDestination(UnitInstance->GetGridIndex(),UnitInstance->GetAbilityTargetGridIndex());
	FIntPoint TargetGridIndex = UnitInstance->GetAbilityTargetGridIndex();
	const FTileData* TileDataP = PawnInstance->GetMyGrid()->GetTileDataByIndex(TargetGridIndex);
	if(TileDataP != nullptr)
	{
		UnitInstance->FaceToTarget(TileDataP->Transform.GetLocation());
	}
	//计算转向
	// const FIntPoint& TargetGridIndex = UnitInstance->GetAbilityTargetGridIndex();
	// const FIntPoint& UnitGridIndex = UnitInstance->GetGridIndex();
	//计算方向 需要旋转角度
	// int DeltaX = TargetGridIndex.X - UnitGridIndex.X;
	// int DeltaY = TargetGridIndex.Y - UnitGridIndex.Y;
	// if(DeltaX == 0)
	// {//左右问题
	// 	UnitGridIndex.Y < TargetGridIndex.Y ? UnitInstance->TurnRight() : UnitInstance->TurnLeft();
	// }
	// else if(DeltaY == 0)
	// {//上下问题
	// 	UnitGridIndex.X < TargetGridIndex.X ? UnitInstance->TurnForward() : UnitInstance->TurnBack();
	// }
	// else
	// {
	// 	if(FMath::Abs(DeltaX) > FMath::Abs(DeltaY))
	// 	{//上下问题
	// 		UnitGridIndex.X < TargetGridIndex.X ? UnitInstance->TurnForward() : UnitInstance->TurnBack();
	// 	}
	// 	else
	// 	{//左右问题
	// 		UnitGridIndex.Y < TargetGridIndex.Y ? UnitInstance->TurnRight() : UnitInstance->TurnLeft();
	// 	}
	// }
	
	PawnInstance->SwitchToCalcAnim();
}

void UPawnProcess_Move::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	Super::EnterProcess(Pawn);

	FPathCompleted Completed;
	Completed.BindUObject(this,&UPawnProcess_Move::UnitMoveToTarget);
	UnitInstance = PawnInstance->GetMyCombatSystem()->GetFirstUnit();
	UnitInstance->StartWalkPath(Completed);
	// FIntPoint StartLocation = UnitInstance->GetGridIndex();
	// FIntPoint FinishLocation = UnitInstance->GetTempDestination();
	// int UnitSide = UnitInstance->GetRuntimeProperty().UnitSide;
	// PawnInstance->GetMyGridPathFinding()->UnitFindPath(UnitSide,StartLocation,FinishLocation,UnitInstance->UnitCanWalkTileType(),Completed);
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
