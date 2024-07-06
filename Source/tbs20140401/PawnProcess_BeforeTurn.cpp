// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_BeforeTurn.h"
#include "My_Pawn.h"
#include "MyCombatSystem.h"
#include "MyUnit.h"


void UPawnProcess_BeforeTurn::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	UE_LOG(LogTemp,Log,TEXT("UPawnProcess_BeforeTurn::EnterProcess"))
	Super::EnterProcess(Pawn);

	const auto MyCombatSystem = PawnInstance->GetMyCombatSystem();
	// const auto MyPathFinding = PawnInstance->GetMyGridPathFinding();
	const auto PreUnit = MyCombatSystem->GetFirstUnit();
	if(PreUnit == nullptr)
	{//PreUnitが何も指し示さない場合なら　始めてということ
		//すべてのオブジェクトの攻撃の範囲計算しておこうと思うんです
		auto AllUnit = MyCombatSystem->GetAllUnits();
		for(auto one : AllUnit)
		{
			one->FinishTurn(false);
		}
	}
	else
	{
		PreUnit->FinishTurn(true);
	}
	
	auto Unit = MyCombatSystem->SortActionPriority();
	if(Unit == nullptr)return;
	Unit->BeforeStartTurn();
	
	PawnInstance->UpdateTileStatusByIndex(Unit->GetGridIndex(),ETileState::Selected);
}

void UPawnProcess_BeforeTurn::TickProcess()
{
	Super::TickProcess();

	if(PawnInstance->ReadStory())
	{
		PawnInstance->SwitchToNormal();		
	}
	else
	{
		PawnInstance->SwitchToTellStory();
	}
	
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
