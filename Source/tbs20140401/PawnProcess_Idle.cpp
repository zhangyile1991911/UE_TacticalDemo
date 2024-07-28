// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_Idle.h"

#include "EventCenter.h"
#include "IdleDirection.h"
#include "MyCombatSystem.h"
#include "MyUnit.h"
#include "My_Pawn.h"

void UPawnProcess_Idle::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	Super::EnterProcess(Pawn);
	UE_LOG(LogTemp,Log,TEXT("UPawnProcess_Idle::EnterProcess"))
	UnitInstance = PawnInstance->GetMyCombatSystem()->GetFirstUnit();
	UnitInstance->ShowDirectionArrow();
	RotateShadow = UnitInstance->NeedToMove();

	auto Direction = UnitInstance->GetUnitDirect();
	switch (Direction)
	{
	case EUnitDirectType::LEFT:
		UnitInstance->GetMyDirection()->DoLeftArrowAnimation();
		break;
	case EUnitDirectType::RIGHT:
		UnitInstance->GetMyDirection()->DoRightArrowAnimation();
		break;
	case EUnitDirectType::FORWARD:
		UnitInstance->GetMyDirection()->DoUpArrowAnimation();
		break;
	case EUnitDirectType::BACKWARD:
		UnitInstance->GetMyDirection()->DoDownArrowAnimation();
		break;
	}
	UnitInstance->SetTempIdleDirection(Direction);
	PawnInstance->GetEventCenter()->EventOfProcessChanged.Broadcast(FText::FromName(TEXT("待機方向を選択")));
}

void UPawnProcess_Idle::TickProcess()
{
	Super::TickProcess();
}

void UPawnProcess_Idle::HandleDirectionInput(const FVector2D& Input)
{
	Super::HandleDirectionInput(Input);

	FVector2D Direction(0,0);
	ECameraDirectType CameraDirect = PawnInstance->GetCurrentCameraDirect();
	switch (CameraDirect)
	{
	case ECameraDirectType::FORWARD:
		Direction = Input;
		break;
	case ECameraDirectType::BACKWARD:
		Direction = Input * -1;
		break;
	case ECameraDirectType::LEFT:
		Direction.X = Input.Y;
		Direction.Y = Input.X * -1;
		break;
	case ECameraDirectType::RIGHT:
		Direction.X = Input.Y * -1;
		Direction.Y = Input.X;
		break;
	}
	
	UE_LOG(LogTemp,Log,TEXT("Input %s"),*Input.ToString())
	if (Direction.Y > 0)
	{
		UnitInstance->GetMyDirection()->DoUpArrowAnimation();
		if(RotateShadow)
		{
			UnitInstance->TurnShadowForward();
		}
		else
		{
			UnitInstance->TurnForward();	
		}
		UnitInstance->SetTempIdleDirection(EUnitDirectType::FORWARD);
	}
	else if(Direction.Y < 0)
	{
		UnitInstance->GetMyDirection()->DoDownArrowAnimation();
		if(RotateShadow)
		{
			UnitInstance->TurnShadowBack();
		}
		else
		{
			UnitInstance->TurnBack();	
		}
		UnitInstance->SetTempIdleDirection(EUnitDirectType::BACKWARD);
	}
	else if(Direction.X > 0)
	{
		UnitInstance->GetMyDirection()->DoRightArrowAnimation();
		if(RotateShadow)
		{
			UnitInstance->TurnShadowRight();
		}
		else
		{
			UnitInstance->TurnRight();	
		}
		UnitInstance->SetTempIdleDirection(EUnitDirectType::RIGHT);
	}
	else if(Direction.X < 0)
	{
		UnitInstance->GetMyDirection()->DoLeftArrowAnimation();
		if(RotateShadow)
		{
			UnitInstance->TurnShadowLeft();
		}
		else
		{
			UnitInstance->TurnLeft();	
		}
		UnitInstance->SetTempIdleDirection(EUnitDirectType::LEFT);
	}
}

void UPawnProcess_Idle::HandleCancelInput()
{
	Super::HandleCancelInput();

	if(UnitInstance->CanAttack() || UnitInstance->CanWalk())
	{
		PawnInstance->SwitchToNormal();
	}
}

void UPawnProcess_Idle::HandleConfirmInput()
{
	Super::HandleConfirmInput();
	if(UnitInstance->NeedToMove())
	{
		UnitInstance->HideShadowUnit();
		PawnInstance->SwitchToMove();
	}
	else
	{
		//清理掉当前选中的格子
		PawnInstance->RemoveCurrentSelectedTile();
		// PawnInstance->SwitchToBeforeTurn();
		PawnInstance->SwitchToFinishTurn();
	}

}

void UPawnProcess_Idle::ExitProcess()
{
	UE_LOG(LogTemp,Log,TEXT("UPawnProcess_Idle::ExitProcess"))
	Super::ExitProcess();
	UnitInstance->HideDirectionArrow();
}

void UPawnProcess_Idle::HandleLeftInput()
{
	PawnInstance->CameraControlLeft();
}

void UPawnProcess_Idle::HandleRightInput()
{
	PawnInstance->CameraControlRight();
}

void UPawnProcess_Idle::HandleZooming(float Val)
{
	PawnInstance->CameraControlZooming(Val);
}
