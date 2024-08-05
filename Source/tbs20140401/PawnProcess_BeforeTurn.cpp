// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_BeforeTurn.h"

#include "FStageData.h"
#include "My_Pawn.h"
#include "MyCombatSystem.h"
#include "MyUnit.h"
#include "MyGameInstance.h"


void UPawnProcess_BeforeTurn::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	UE_LOG(LogTemp,Log,TEXT("UPawnProcess_BeforeTurn::EnterProcess"))
	Super::EnterProcess(Pawn);

	const auto MyCombatSystem = PawnInstance->GetMyCombatSystem();
	// const auto MyPathFinding = PawnInstance->GetMyGridPathFinding();
	const auto PreUnit = MyCombatSystem->GetFirstUnit();
	const bool IsFirstTurn = PreUnit == nullptr;

	TArray<TObjectPtr<AMyUnit>> AllUnit = MyCombatSystem->GetAllUnits();
	for(auto one : AllUnit)
	{
		if(one->IsDead() == false)
		{
			UE_LOG(LogTemp,Log,TEXT("UPawnProcess_BeforeTurn::EnterProcess %d"),one->GetUnitType());
			one->CalcUnitArea(false);
		}
	}
	if(!IsFirstTurn)
	{
		PreUnit->FinishTurn();
	}
	// if(PreUnit == nullptr)
	// {//PreUnitが何も指し示さない場合なら　始めてということ
	// 	//すべてのオブジェクトの攻撃の範囲計算しておこうと思うんです
	// 	auto AllUnit = MyCombatSystem->GetAllUnits();
	// 	for(auto one : AllUnit)
	// 	{
	// 		one->FinishTurn(false);
	// 	}
	// }
	// else
	// {
	// 	PreUnit->FinishTurn(true);
	// }
	
	auto Unit = MyCombatSystem->SortActionPriority();
	if(Unit == nullptr)return;
	Unit->BeforeStartTurn();
	
	PawnInstance->UpdateTileStatusByIndex(Unit->GetGridIndex(),ETileState::Selected);
	
	if(GameInstance == nullptr)
	{
		GameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	}
	
	int LevelNum = PawnInstance->GetStageLevelNum();
	FStageData StageData = GameInstance->GetStageData(LevelNum);
	StageData.ResOfDialogue.LoadSynchronous();
	if(StageData.ResOfDialogue.IsValid() && ReadStoryNum != LevelNum)
	{
		ReadStoryNum = LevelNum;
		PawnInstance->SwitchToTellStory();
	}
	else
	{
		PawnInstance->SwitchToNormal();
	}
	
	// if(PawnInstance->ReadStory())
	// {
	// 	PawnInstance->SwitchToNormal();		
	// }
	// else
	// {
	// 	PawnInstance->SwitchToTellStory();
	// }
	
	// PawnInstance->SwitchToNormal();
}

void UPawnProcess_BeforeTurn::TickProcess()
{
	Super::TickProcess();
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
