// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_Idle.h"

#include "IdleDirection.h"
#include "MyCombatSystem.h"
#include "MyUnit.h"
#include "My_Pawn.h"

void UPawnProcess_Idle::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	Super::EnterProcess(Pawn);
	UnitInstance = PawnInstance->GetMyCombatSystem()->GetFirstUnit();
	UnitInstance->ShowDirectionArrow();
}

void UPawnProcess_Idle::TickProcess()
{
	Super::TickProcess();
}

void UPawnProcess_Idle::HandleDirectionInput(const FVector2D& Input)
{
	Super::HandleDirectionInput(Input);
	UE_LOG(LogTemp,Log,TEXT("Input %s"),*Input.ToString())
	if (Input.Y > 0)
	{
		UnitInstance->GetMyDirection()->DoUpArrowAnimation();
		UnitInstance->TurnForward();
	}
	else if(Input.Y < 0)
	{
		UnitInstance->GetMyDirection()->DoDownArrowAnimation();
		UnitInstance->TurnBack();
	}
	else if(Input.X > 0)
	{
		UnitInstance->GetMyDirection()->DoRightArrowAnimation();
		UnitInstance->TurnRight();
	}
	else if(Input.X < 0)
	{
		UnitInstance->GetMyDirection()->DoLeftArrowAnimation();
		UnitInstance->TurnLeft();
	}
}

void UPawnProcess_Idle::HandleCancelInput()
{
	Super::HandleCancelInput();
	PawnInstance->SwitchToNormal();
}

void UPawnProcess_Idle::HandleConfirmInput()
{
	Super::HandleConfirmInput();
	PawnInstance->SwitchToBeforeTurn();
}

void UPawnProcess_Idle::ExitProcess()
{
	Super::ExitProcess();
	UnitInstance->HideDirectionArrow();
}
