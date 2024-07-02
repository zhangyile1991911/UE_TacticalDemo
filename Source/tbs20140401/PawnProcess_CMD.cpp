// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_CMD.h"

#include "BottomActionBar.h"
#include "CmdWidget.h"
#include "MyHUD.h"
#include "My_Pawn.h"
#include "MyCombatSystem.h"
#include "MyUnit.h"
// #include "UnitAbility.h"
#include "BattleFunc.h"
#include "EventCenter.h"
#include "Grid.h"
#include "UGameUI_UnitBreifInfo.h"
#include "UnitAbilityAnim.h"

void UPawnProcess_CMD::ShowBriefInfo()
{
	auto ChosenAbilityPtr = UnitInstance->GetChosenAbilityAnim();
	if(ChosenAbilityPtr->IsIdle())
	{
		UnitBriefInfoPtr->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	//显示可以攻击范围
	ClearAbilityRange();

	TObjectPtr<AMyUnit> TargetUnit = nullptr;
	if(UnitInstance->NeedToMove())
	{
		ArrayOfAbilityRange = ChosenAbilityPtr->Range(UnitInstance->GetTempDestinationGridIndex());
	}
	else
	{
		ArrayOfAbilityRange = ChosenAbilityPtr->Range(UnitInstance->GetGridIndex());	
	}
	
	for(const FIntPoint& one : ArrayOfAbilityRange)
	{
		PawnInstance->GetMyGrid()->AddStateToTile(one,ETileState::AbilityRange);

		//治疗或者范围攻击 都不会自动选择目标
		if(ChosenAbilityPtr->IsArea()||ChosenAbilityPtr->IsHeal())continue;
		//已经有目标就跳过
		if(TargetUnit != nullptr)continue;
		
		const FTileData* TileDataPtr = PawnInstance->GetMyGrid()->GetTileDataByIndex(one);
		if(TileDataPtr == nullptr)continue;

		if(TileDataPtr->UnitOnTile == nullptr)continue;

		if(!TileDataPtr->UnitOnTile->IsFriend(UnitInstance->GetUnitSide()))
			TargetUnit = TileDataPtr->UnitOnTile;
	}

	UnitBriefInfoPtr->SetVisibility(ESlateVisibility::Visible);
	
	//如果是范围攻击或者治疗 都显示自己
	if(ChosenAbilityPtr->IsArea()||ChosenAbilityPtr->IsHeal())
	{
		UnitBriefInfoPtr->ShowDetailInfoOnly(UnitInstance);
		return;	
	}
	if(TargetUnit == nullptr)
	{
		UnitBriefInfoPtr->ShowDetailInfoOnly(UnitInstance);
		return;	
	}
	
	const bool bHasWrap = UBattleFunc::HasWrapAttackUnit(UnitInstance,TargetUnit,PawnInstance->GetMyGrid()) != nullptr;
	const bool bIsBack = UBattleFunc::IsBackAttack(UnitInstance,TargetUnit);
	const float HitPercent = UBattleFunc::CalculateHitRate(UnitInstance,TargetUnit,PawnInstance->GetMyGrid(),bIsBack,bHasWrap);
	UnitBriefInfoPtr->ShowTargetInfoAndTab(TargetUnit,HitPercent);
	
}

void UPawnProcess_CMD::ClearAbilityRange()
{
	for(const FIntPoint& one : ArrayOfAbilityRange)
	{
		PawnInstance->GetMyGrid()->RemoveStateFromTile(one,ETileState::AbilityRange);
	}
	ArrayOfAbilityRange.Empty();
}

void UPawnProcess_CMD::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	UE_LOG(LogTemp,Log,TEXT("UPawnProcess_CMD::EnterProcess"))
	Super::EnterProcess(Pawn);
	CmdIndex = 0;
	UnitInstance = PawnInstance->GetMyCombatSystem()->GetFirstUnit();
	//GameUIの事例を取得します
	BottomActionBarInstance = PawnInstance->GetMyHUD()->GetGameUI();
	//命令パネルを表示します
	CmdWidgetInstance = BottomActionBarInstance->ShowCmdPanel(UnitInstance,CmdIndex);

	auto Tmp = PawnInstance->GetMyHUD()->GetGameUI();
	UnitBriefInfoPtr = Tmp->GetUnitBriefInfo();

	if(UnitInstance->AttackCount() > 1)
	{
		PawnInstance->GetEventCenter()->EventOfProcessChanged.Broadcast(FText::FromName(TEXT("最初のコマンドを選択")));
	}
	else
	{
		PawnInstance->GetEventCenter()->EventOfProcessChanged.Broadcast(FText::FromName(TEXT("コマンドを選択")));
	}
	
	// CmdWidgetInstance = BottomActionBarInstance->GetCmdPanel();
	// CmdWidgetInstance->SetVisibility(ESlateVisibility::Visible);
	// CmdWidgetInstance->RefreshUnitCmd(UnitInstance);
	// CmdWidgetInstance->SelectCmd(CmdIndex);
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
	NextCmdIndex = FMath::Clamp(NextCmdIndex,0,UnitInstance->GetSelectableAbilityNum());
	// UE_LOG(LogTemp,Log,TEXT("after CmdIndex = %d"),CmdIndex);
	CmdWidgetInstance->SelectCmd(NextCmdIndex);
	UnitInstance->SetChosenAbility(NextCmdIndex);
	CmdIndex = NextCmdIndex;

	ShowBriefInfo();
}

void UPawnProcess_CMD::HandleCancelInput()
{
	Super::HandleCancelInput();
	UE_LOG(LogTemp,Log,TEXT("UPawnProcess_CMD::HandleCancelInput()"))
	if(UnitInstance->HasAttackDone())
	{
		return;
	}
	PawnInstance->SwitchToNormal();
}

void UPawnProcess_CMD::HandleConfirmInput()
{
	Super::HandleConfirmInput();

	const TObjectPtr<AUnitAbilityAnim> ChosenAbility = UnitInstance->GetChosenAbilityAnim();
	//是否可以执行
	if(!ChosenAbility->CanExecute())
	{
		return;
	}
	UnitInstance->SetChosenAbility(CmdIndex);
	
	PawnInstance->SwitchToChooseTarget();
	// if(ChosenAbility->GetSkillData().SkillId == 10001)
	// {
	// 	PawnInstance->SwitchToIdle();
	// }
	// else
	// {
	// 	PawnInstance->SwitchToChooseTarget();
	// }
	
}

void UPawnProcess_CMD::HandleSpaceInput()
{
	PawnInstance->SwitchToIdle();
}

void UPawnProcess_CMD::ExitProcess()
{
	UE_LOG(LogTemp,Log,TEXT("UPawnProcess_CMD::ExitProcess"))
	Super::ExitProcess();
	CmdWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	UnitBriefInfoPtr->SetVisibility(ESlateVisibility::Hidden);
	BottomActionBarInstance = nullptr;
	CmdWidgetInstance = nullptr;
	UnitInstance->HideShadowUnit();
	UnitInstance = nullptr;

	ClearAbilityRange();
}
