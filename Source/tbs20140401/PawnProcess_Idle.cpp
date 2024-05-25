// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_Idle.h"

#include "My_Pawn.h"

void UPawnProcess_Idle::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	Super::EnterProcess(Pawn);
}

void UPawnProcess_Idle::TickProcess()
{
	Super::TickProcess();
}

void UPawnProcess_Idle::HandleDirectionInput(const FVector2D& Input)
{
	Super::HandleDirectionInput(Input);
	
}

void UPawnProcess_Idle::HandleCancelInput()
{
	Super::HandleCancelInput();
	PawnInstance->SwitchToNormal();
}

void UPawnProcess_Idle::HandleConfirmInput()
{
	Super::HandleConfirmInput();
}

void UPawnProcess_Idle::ExitProcess()
{
	Super::ExitProcess();
}
