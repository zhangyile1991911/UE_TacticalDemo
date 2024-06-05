// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_BeforeTurn.h"
#include "My_Pawn.h"
#include "MyCombatSystem.h"
#include "MyUnit.h"


void UPawnProcess_BeforeTurn::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	UE_LOG(LogTemp,Log,TEXT("UPawnProcess_BeforeTurn::EnterProcess"))
	Super::EnterProcess(Pawn);
	//第一回合 会是空
	auto PreUnit =PawnInstance->GetMyCombatSystem()->GetFirstUnit();
	if(PreUnit != nullptr)PreUnit->FinishTurn();
	
	auto Unit = PawnInstance->GetMyCombatSystem()->SortActionPriority();
	if(Unit == nullptr)return;
	Unit->BeforeStartTurn();
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
	UE_LOG(LogTemp,Log,TEXT("UPawnProcess_BeforeTurn::ExitProcess"))
	Super::ExitProcess();
}
