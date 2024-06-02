// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_CMD.h"

#include "BottomActionBar.h"
#include "CmdWidget.h"
#include "MyHUD.h"
#include "My_Pawn.h"
#include "MyCombatSystem.h"
#include "MyUnit.h"
#include "UnitAbility.h"

void UPawnProcess_CMD::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	Super::EnterProcess(Pawn);
	CmdIndex = 0;
	UnitInstance = PawnInstance->GetMyCombatSystem()->GetFirstUnit();
	//GameUIの事例を取得します
	BottomActionBarInstance = PawnInstance->GetMyHUD()->GetGameUI();
	//命令パネルを表示します
	CmdWidgetInstance = BottomActionBarInstance->GetCmdPanel();
	CmdWidgetInstance->SetVisibility(ESlateVisibility::Visible);
	CmdWidgetInstance->RefreshUnitCmd(UnitInstance);
	CmdWidgetInstance->SelectCmd(CmdIndex);
}

void UPawnProcess_CMD::TickProcess()
{
	Super::TickProcess();
}

void UPawnProcess_CMD::HandleDirectionInput(const FVector2D& Index)
{
	Super::HandleDirectionInput(Index);
	// UE_LOG(LogTemp,Log,TEXT("x = %f y = %f"),Index.X,Index.Y);
	// UE_LOG(LogTemp,Log,TEXT("before CmdIndex = %d"),CmdIndex);
	int NextCmdIndex = CmdIndex - Index.Y;
	NextCmdIndex = FMath::Clamp(NextCmdIndex,0,UnitInstance->GetOwnAbilityList().Num()-1);
	// UE_LOG(LogTemp,Log,TEXT("after CmdIndex = %d"),CmdIndex);
	CmdWidgetInstance->SelectCmd(NextCmdIndex);
	CmdIndex = NextCmdIndex;
}

void UPawnProcess_CMD::HandleCancelInput()
{
	Super::HandleCancelInput();
	UE_LOG(LogTemp,Log,TEXT("UPawnProcess_CMD::HandleCancelInput()"))
	PawnInstance->SwitchToNormal();
}

void UPawnProcess_CMD::HandleConfirmInput()
{
	Super::HandleConfirmInput();

	const TObjectPtr<UUnitAbility> ChosenAbility = UnitInstance->GetOwnAbilityList()[CmdIndex];
	//是否可以执行
	if(!ChosenAbility->CanExecute())
	{
		return;
	}
	UnitInstance->SetChosenAbility(CmdIndex);

	if(ChosenAbility->IsIdle())
	{
		PawnInstance->SwitchToIdle();
	}
	else
	{
		PawnInstance->SwitchToChooseTarget();	
	}
	
	
	// if(ChosenAbility->GetSkillData().SkillId == 10001)
	// {
	// 	PawnInstance->SwitchToIdle();
	// }
	// else
	// {
	// 	PawnInstance->SwitchToChooseTarget();
	// }
	
}

void UPawnProcess_CMD::ExitProcess()
{
	Super::ExitProcess();
	CmdWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	BottomActionBarInstance = nullptr;
	CmdWidgetInstance = nullptr;
	UnitInstance->HideShadowUnit();
}
