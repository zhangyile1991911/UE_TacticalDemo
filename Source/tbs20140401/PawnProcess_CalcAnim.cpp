// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_CalcAnim.h"
#include "My_Pawn.h"
#include "MyCombatSystem.h"
#include "MyUnit.h"
#include "UnitAbilityAnim.h"
#include "UnitAbility.h"
#include "BattleReport.h"

void UPawnProcess_CalcAnim::AbilityCompleted(TObjectPtr<AUnitAbilityAnim> Ability)
{
	UnitInstance->AttackDone();
	PawnInstance->SwitchToIdle();
}

void UPawnProcess_CalcAnim::AbilityCompletedEvent(TObjectPtr<AUnitAbilityAnim> Ability)
{
	
}

void UPawnProcess_CalcAnim::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	Super::EnterProcess(Pawn);
	
	UnitInstance = PawnInstance->GetMyCombatSystem()->GetFirstUnit();
	ChosenAbility = UnitInstance->GetChosenAbility();
	ChosenAbilityAnim = UnitInstance->GetChosenAbilityAnim();
	
	ChosenAbilityAnim->CompletedCallback.BindUObject(this,&UPawnProcess_CalcAnim::AbilityCompleted);
	ChosenAbilityAnim->CompletedEvent.AddUObject(this,&UPawnProcess_CalcAnim::AbilityCompletedEvent);

	FIntPoint TargetLocation = UnitInstance->GetAbilityTargetGridIndex();
	//控制流程

	//目标选择
	TArray<TObjectPtr<AMyUnit>> TargetUnits = ChosenAbility->TakeTargets(TargetLocation,PawnInstance->GetMyGrid());
	
	//计算战报
	Report = ChosenAbility->DoCalculation(TargetUnits,PawnInstance->GetMyGrid());
	//进入演出环节
	ChosenAbilityAnim->DoAnimation(Report,PawnInstance);
	
}

void UPawnProcess_CalcAnim::TickProcess()
{
	Super::TickProcess();
	
}

void UPawnProcess_CalcAnim::HandleDirectionInput(const FVector2D& Input)
{
	Super::HandleDirectionInput(Input);
}

void UPawnProcess_CalcAnim::HandleCancelInput()
{
	Super::HandleCancelInput();
}

void UPawnProcess_CalcAnim::HandleConfirmInput()
{
	Super::HandleConfirmInput();
}

void UPawnProcess_CalcAnim::ExitProcess()
{
	Super::ExitProcess();
	ChosenAbilityAnim->CompletedCallback.Unbind();
	ChosenAbilityAnim->CompletedEvent.Clear();
}
