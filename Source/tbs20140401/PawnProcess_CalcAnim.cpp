// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_CalcAnim.h"
#include "My_Pawn.h"
#include "MyCombatSystem.h"
#include "MyUnit.h"
#include "UnitAbilityAnim.h"
#include "BattleReport.h"

void UPawnProcess_CalcAnim::CheckFlow(FlowControl Control)
{
	bool IsContinue = false;
	switch (Control)
	{
	case IDLE:
		IsContinue = CheckIdle(ChosenAbilityAnim);
    	if(!IsContinue)return;
    	
    	IsContinue = CheckDeath();
    	if(!IsContinue)return;
    	
    	IsContinue = CheckCooperation();
    	if(!IsContinue)return;
    	
    	if(UnitInstance->CanAttack())
    	{
    		PawnInstance->SwitchToCmdInput();
    	}
		break;
	case DEATH:
		// IsContinue = CheckCooperation();
		// if(!IsContinue)return;
    	
		if(UnitInstance->CanAttack())
		{
			PawnInstance->SwitchToCmdInput();
		}
		break;
	case COOPERATION:
		IsContinue = CheckDeath();
		if(!IsContinue)return;
		if(UnitInstance->CanAttack())
		{
			PawnInstance->SwitchToCmdInput();
		}
		break;
	case ATTACK:
		if(UnitInstance->CanAttack())
		{
			PawnInstance->SwitchToCmdInput();
		}
		break;
	}
	ReportIndex++;
	if(ReportIndex < ReportList.Num())
	{
		CheckFlow(IDLE);
	}
	else
	{
		PawnInstance->SwitchToIdle();	
	}
	
}


void UPawnProcess_CalcAnim::OnDeathCompleted()
{
	const FBattleReport Report = ReportList[ReportIndex];
	PawnInstance->GetMyCombatSystem()->RemoveUnitInCombat(Report.Defender);
	CheckFlow(DEATH);
}

bool UPawnProcess_CalcAnim::CheckIdle(TObjectPtr<AUnitAbilityAnim> Ability)
{
	// const FBattleReport& Report = ReportList[ReportIndex];
	if(Ability->IsIdle())
	{
		PawnInstance->SwitchToBeforeTurn();
		return false;
	}
	return true;
}

bool UPawnProcess_CalcAnim::CheckDeath()
{
	AMyUnit* Defender = ReportList[ReportIndex].Defender;
	if(Defender != nullptr)
	{
		if(Defender->IsDead())
		{
			FDeathCompleted DeathCompleted;
			DeathCompleted.BindUObject(this,&UPawnProcess_CalcAnim::OnDeathCompleted);
			Defender->DoDeadAnim(DeathCompleted);
			return false;
		}	
	}
	return true;
}

bool UPawnProcess_CalcAnim::CheckCooperation()
{
	// if(Report.Cooperator != nullptr)
	// {//夹击
	// 	ChosenAbilityAnim = Report.Cooperator->GetChosenAbilityAnim();
	// 	FIntPoint TargetLocation = Report.Defender->GetGridIndex();
	// 	TArray<TObjectPtr<AMyUnit>> TargetUnits = ChosenAbilityAnim->TakeTargets(TargetLocation,PawnInstance->GetMyGrid());
	// 	Report = ChosenAbilityAnim->DoCalculation(TargetUnits,PawnInstance->GetMyGrid(),false);
	// 	ChosenAbilityAnim->DoAnimation(Report,PawnInstance);
	// 	return;
	// }
	return true;
}

void UPawnProcess_CalcAnim::AbilityCompleted(TObjectPtr<AUnitAbilityAnim> Ability)
{
	UnitInstance->AttackDone();
	
	CheckFlow(IDLE);
}

// void UPawnProcess_CalcAnim::AbilityCompletedEvent(TObjectPtr<AUnitAbilityAnim> Ability)
// {
// 	
// }

void UPawnProcess_CalcAnim::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	Super::EnterProcess(Pawn);
	
	UnitInstance = PawnInstance->GetMyCombatSystem()->GetFirstUnit();
	
	// UnitInstance = PawnInstance->GetMyCombatSystem()->GetFirstUnit();
	// ChosenAbility = UnitInstance->GetChosenAbility();
	ChosenAbilityAnim = UnitInstance->GetChosenAbilityAnim();
	
	ChosenAbilityAnim->CompletedCallback.BindUObject(this,&UPawnProcess_CalcAnim::AbilityCompleted);
	// ChosenAbilityAnim->CompletedEvent.AddUObject(this,&UPawnProcess_CalcAnim::AbilityCompletedEvent);
	
	FIntPoint TargetLocation = UnitInstance->GetAbilityTargetGridIndex();
	//控制流程
	
	//目标选择
	TArray<TObjectPtr<AMyUnit>> TargetUnits = ChosenAbilityAnim->TakeTargets(TargetLocation,PawnInstance->GetMyGrid());
	
	//计算战报
	ReportList = ChosenAbilityAnim->DoCalculation(TargetUnits,PawnInstance->GetMyGrid(),true);
	ReportIndex = 0;
	//进入演出环节
	ChosenAbilityAnim->DoAnimation(ReportList[ReportIndex],PawnInstance);
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
	ReportList.Empty();
	ChosenAbilityAnim->CompletedCallback.Unbind();
	ChosenAbilityAnim->CompletedEvent.Clear();
}
