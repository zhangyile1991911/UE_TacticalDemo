// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_FinishTurn.h"

#include "MyCombatSystem.h"
#include "MyUnit.h"
#include "My_Pawn.h"

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
		UE_LOG(LogTemp,Log,TEXT(""))
		
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
