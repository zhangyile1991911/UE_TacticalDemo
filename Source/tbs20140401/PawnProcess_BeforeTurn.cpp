// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_BeforeTurn.h"
#include "My_Pawn.h"
#include "MyCombatSystem.h"
#include "MyUnit.h"


void UPawnProcess_BeforeTurn::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	Super::EnterProcess(Pawn);

	auto Unit = PawnInstance->GetMyCombatSystem()->SortActionPriority();
	PawnInstance->UpdateTileStatusByIndex(Unit->GetGridIndex(),ETileState::Selected);
}

void UPawnProcess_BeforeTurn::TickProcess()
{
	Super::TickProcess();
	
	PawnInstance->SwitchToNormal();
}

void UPawnProcess_BeforeTurn::HandleDirectionInput(const FVector2D& Input)
{
	Super::HandleDirectionInput(Input);
}

void UPawnProcess_BeforeTurn::HandleCancelInput()
{
	Super::HandleCancelInput();
}

void UPawnProcess_BeforeTurn::HandleConfirmInput()
{
	Super::HandleConfirmInput();
}

void UPawnProcess_BeforeTurn::ExitProcess()
{
	Super::ExitProcess();
}
