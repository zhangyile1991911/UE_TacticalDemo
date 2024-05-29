// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_ChooseTarget.h"

#include "My_Pawn.h"
#include "MyCombatSystem.h"
#include "MyUnit.h"
#include "Grid.h"
#include "MyGridPathfinding.h"
#include "UnitAbility.h"
#include "UnitAbilityAnim.h"



void UPawnProcess_ChooseTarget::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	Super::EnterProcess(Pawn);
	UnitInstance = PawnInstance->GetMyCombatSystem()->GetFirstUnit();
	UnitInstance->ShowShadowUnit();
	ChosenAbility = UnitInstance->GetChosenAbility();
	// ChosenAbilityAnim = UnitInstance->GetChosenAbilityAnim();

	CurrentCursor = PawnInstance->GetSelectedTile();
	//显示攻击范围
	AbilityRange = ChosenAbility->Range(CurrentCursor);
	for(const FIntPoint& one : AbilityRange)
	{
		PawnInstance->GetMyGrid()->AddStateToTile(one,ETileState::AbilityRange);
	}
	
}

void UPawnProcess_ChooseTarget::TickProcess()
{
	Super::TickProcess();
}

void UPawnProcess_ChooseTarget::HandleDirectionInput(const FVector2D& Input)
{
	Super::HandleDirectionInput(Input);

	FIntPoint next;
	next.X = CurrentCursor.X + Input.Y;
	next.Y = CurrentCursor.Y + Input.X;

	if(PawnInstance->GetMyGrid()->IsValidGridIndex(next))
	{
		PawnInstance->GetMyGrid()->RemoveStateFromTile(CurrentCursor,ETileState::Selected);
		PawnInstance->GetMyGrid()->AddStateToTile(next,ETileState::Selected);
		CurrentCursor = next;
		UE_LOG(LogTemp,Log,TEXT(" HandleDirectionInput CurrentCursor (%d,%d)"),CurrentCursor.X,CurrentCursor.Y);
	}
	
}

void UPawnProcess_ChooseTarget::HandleCancelInput()
{
	Super::HandleCancelInput();
	PawnInstance->SwitchToCmdInput();
}

void UPawnProcess_ChooseTarget::HandleConfirmInput()
{
	Super::HandleConfirmInput();
	auto TileData = PawnInstance->GetMyGrid()->GetTileDataByIndex(CurrentCursor);
	
	if(!AbilityRange.Contains(CurrentCursor))return;

	
	if(!ChosenAbility->IsValidTarget(*TileData))return;

	// ChosenAbilityAnim->CompletedCallback.BindUObject(this,&UPawnProcess_ChooseTarget::AbilityCompleted);
	// ChosenAbilityAnim->CompletedEvent.AddUObject(this,&UPawnProcess_ChooseTarget::AbilityCompletedEvent);

	//控制流程
	//todo 目标选择
	TArray<TObjectPtr<AMyUnit>> TargetUnits = ChosenAbility->TakeTargets(CurrentCursor,PawnInstance->GetMyGrid());
	
	//计算战报
	// FBattleReport Report = ChosenAbility->DoCalculation(TargetUnits,PawnInstance->GetMyGrid());
	//todo 进入演出环节
	// ChosenAbilityAnim->DoAnimation(Report,PawnInstance);

	
}

void UPawnProcess_ChooseTarget::ExitProcess()
{
	Super::ExitProcess();
	
	for(const FIntPoint& one : AbilityRange)
	{
		PawnInstance->GetMyGrid()->RemoveStateFromTile(one,ETileState::AbilityRange);
	}
	PawnInstance->GetMyGrid()->RemoveStateFromTile(CurrentCursor,ETileState::Selected);
	PawnInstance->UpdateTileStatusByIndex(CurrentCursor,ETileState::Selected);
	
}


// void UPawnProcess_ChooseTarget::AbilityCompleted(TObjectPtr<AUnitAbilityAnim> Ability)
// {
// 	UE_LOG(LogTemp,Log,TEXT("UPawnProcess_ChooseTarget::AbilityCompleted"))
// 	//todo 是否有 夹击
// 	// if(Report.Cooperator != nullptr)
// 	// {
// 	// 	
// 	// }
// }
//
// void UPawnProcess_ChooseTarget::AbilityCompletedEvent(TObjectPtr<AUnitAbilityAnim> Ability)
// {
// 	UE_LOG(LogTemp,Log,TEXT("UPawnProcess_ChooseTarget::AbilityCompletedEvent"))
// }