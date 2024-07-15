// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_FinishTurn.h"

#include "FStageData.h"
#include "GameSystemPanel.h"
#include "Grid.h"
#include "MyCombatSystem.h"
#include "MyHUD.h"
#include "MyUnit.h"
#include "My_Pawn.h"
#include "My_Utilities.h"
#include "Kismet/GameplayStatics.h"

void UPawnProcess_FinishTurn::ClearCurrentStage()
{
	PawnInstance->GetMyGrid()->ClearGrid();
	PawnInstance->GetMyCombatSystem()->ClearAllUnit();
	
	int LevelNum = PawnInstance->GetStageLevelNum();
	
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("OnLevelUnloaded");
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = __LINE__; // 确保每次调用都有唯一的UUID
	
	FStageData* Data = GetStageData(LevelNum);
	UGameplayStatics::UnloadStreamLevel(GetWorld(),FName(Data->StageLevelName.ToString()),LatentInfo,false);
}

void UPawnProcess_FinishTurn::OnLevelUnloaded()
{
	PawnInstance->NextStageLevel();
	if(PawnInstance->GetStageLevelNum() > MaxLevelNum)
	{//游戏结束
		PawnInstance->SwitchToGameOver();
	}
	else
	{
		PawnInstance->SwitchToLoadStage();	
	}
	
}

void UPawnProcess_FinishTurn::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	Super::EnterProcess(Pawn);
	//1 判断是否有一方队伍 战斗单位都死了
	auto AllUnit = PawnInstance->GetMyCombatSystem()->GetAllUnits();
	TSet<int> UnitSide;
	for(auto OneUnit : AllUnit)
	{
		int Side = OneUnit->GetUnitSide();
		if(UnitSide.Contains(Side))
		{
			continue;
		}
		UnitSide.Add(Side);
	}

	if(UnitSide.Num() <= 1)
	{//只剩下一個隊伍
		UE_LOG(LogTemp,Log,TEXT("当前关卡结束"))
		GameSystemPanel = PawnInstance->GetMyHUD()->GetGameSystemPanel();
		GameSystemPanel->ShowLoading();
		ClearCurrentStage();
	}
	else
	{//繼續
		PawnInstance->SwitchToBeforeTurn();
	}
}

void UPawnProcess_FinishTurn::ExitProcess()
{
	Super::ExitProcess();
}

void UPawnProcess_FinishTurn::HandleConfirmInput()
{
	Super::HandleConfirmInput();

	
}
