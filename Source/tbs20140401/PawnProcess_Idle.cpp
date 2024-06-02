﻿// Fill out your copyright notice in the Description page of Project Settings.


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
	RotateShadow = UnitInstance->NeedToMove(); 
	if(RotateShadow)
	{
		UnitInstance->ShowShadowUnit();
	}
	else
	{
		UnitInstance->HideShadowUnit();
	}
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
		if(RotateShadow)
		{
			UnitInstance->TurnShadowForward();
		}
		else
		{
			UnitInstance->TurnForward();	
		}
		
	}
	else if(Input.Y < 0)
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
		
	}
	else if(Input.X > 0)
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
		
	}
	else if(Input.X < 0)
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
	}
}

void UPawnProcess_Idle::HandleCancelInput()
{
	Super::HandleCancelInput();
	if(UnitInstance->CanWalk())
	{
		PawnInstance->SwitchToNormal();
	}
	else if(UnitInstance->CanAttack() && UnitInstance->CanWalk())
	{
		PawnInstance->SwitchToNormal();
	}
		
}

void UPawnProcess_Idle::HandleConfirmInput()
{
	Super::HandleConfirmInput();
	UnitInstance->FinishTurn();
	PawnInstance->SwitchToBeforeTurn();
}

void UPawnProcess_Idle::ExitProcess()
{
	Super::ExitProcess();
	UnitInstance->HideDirectionArrow();
}
