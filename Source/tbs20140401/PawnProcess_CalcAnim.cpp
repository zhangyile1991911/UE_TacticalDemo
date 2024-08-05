// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_CalcAnim.h"
#include "My_Pawn.h"
#include "MyCombatSystem.h"
#include "MyUnit.h"
#include "UnitAbilityAnim.h"
#include "BattleReport.h"
#include "BottomActionBar.h"
#include "MyHUD.h"

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
    		return;
    	}
		PawnInstance->SwitchToIdle();
		break;
	case DEATH:
		// IsContinue = CheckCooperation();
		// if(!IsContinue)return;
    	
		if(UnitInstance->CanAttack())
		{
			PawnInstance->SwitchToCmdInput();
			return;
		}
		PawnInstance->SwitchToIdle();
		break;
	case COOPERATION:
		IsContinue = CheckDeath();
		if(!IsContinue)return;
		if(UnitInstance->CanAttack())
		{
			PawnInstance->SwitchToCmdInput();
			return;
		}
		PawnInstance->SwitchToIdle();
		break;
	// case ATTACK:
	// 	if(UnitInstance->CanAttack())
	// 	{
	// 		PawnInstance->SwitchToCmdInput();
	// 	}
	// 	break;
	}
	
	// ReportIndex++;
	// if(ReportIndex < ReportList.Num())
	// {
	// 	CheckFlow(IDLE);
	// }
	// else
	// {
	// 	PawnInstance->SwitchToIdle();	
	// }
	
}


void UPawnProcess_CalcAnim::OnDeathCompleted()
{
	DeathNum -= 1;
	if(DeathNum > 0)return;
	
	if(Report.Defender != nullptr)
	{
		PawnInstance->GetMyCombatSystem()->RemoveUnitInCombat(Report.Defender);
		PawnInstance->OnRemoveUnit(Report.Defender);
	}

	if(!Report.HitInfoList.IsEmpty())
	{
		for(int i = 0;i < Report.HitInfoList.Num();i++)
		{
			auto one = Report.HitInfoList[i].Defender;
			if(one->IsDead())
			{
				PawnInstance->GetMyCombatSystem()->RemoveUnitInCombat(one);
				PawnInstance->OnRemoveUnit(one);	
			}
		}
	}
	
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
	// AMyUnit* Defender = ReportList[ReportIndex].Defender;
	// AMyUnit* Defender = Report.Defender;
	// if(Defender != nullptr)
	// {
	bool HasDead = false;
	DeathNum = 0;

	if(Report.Defender != nullptr && Report.Defender->IsDead())
	{
		FDeathCompleted DeathCompleted;
		DeathCompleted.BindUObject(this,&UPawnProcess_CalcAnim::OnDeathCompleted);
		Report.Defender->DoDeadAnim(DeathCompleted);
		HasDead = true;
		DeathNum = 1;
	}
	if(!Report.HitInfoList.IsEmpty())
	{
		for(int i = 0; i < Report.HitInfoList.Num();i++)
		{
			if(!Report.HitInfoList[i].Defender->IsDead())continue;
			if(Report.HitInfoList[i].ProcessIndex == 1)
			{
				FDeathCompleted DeathCompleted;
				DeathCompleted.BindUObject(this,&UPawnProcess_CalcAnim::OnDeathCompleted);
				Report.HitInfoList[i].Defender->DoDeadAnim(DeathCompleted);
				Report.HitInfoList[i].ProcessIndex = 2;
				DeathNum++;
				HasDead = true;
				UE_LOG(LogTemp,Log,TEXT("UPawnProcess_CalcAnim::CheckDeath() %s"),*Report.HitInfoList[i].Defender->GetProperty().UnitName.ToString())	
			}
		}
	}
	return !HasDead;
}

bool UPawnProcess_CalcAnim::CheckCooperation()
{
	// const FBattleReport& Report = ReportList[ReportIndex];
	if(Report.Cooperator != nullptr)
	{//挟み撃ち
		ChosenAbilityAnim = Report.Cooperator->GetCooperationAbilityAnim();
		// FIntPoint TargetLocation = Report.Defender[0]->GetGridIndex();
		// TArray<TObjectPtr<AMyUnit>> TargetUnits = ChosenAbilityAnim->TakeTargets(TargetLocation,PawnInstance->GetMyGrid());
		Report = ChosenAbilityAnim->DoCalculation(Report.CooperatorTarget,PawnInstance->GetMyGrid(),false);
		// ReportList.Append(CoReport);
		ChosenAbilityAnim->CompletedCallback.BindUObject(this,&UPawnProcess_CalcAnim::AbilityCompleted);
		ChosenAbilityAnim->DoAnimation(Report,PawnInstance);
		return false;
	}
	if(!Report.HitInfoList.IsEmpty())
	{
		for(int i = 0;i >=0;i--)
		{
			auto& LastReport = Report.HitInfoList[i];
			if(LastReport.Cooperator == nullptr)continue;
			if(LastReport.ProcessIndex == 0)
			{
				ChosenAbilityAnim = LastReport.Cooperator->GetCooperationAbilityAnim();
				const auto CoReport = ChosenAbilityAnim->DoCalculation(LastReport.CooperatorTarget,PawnInstance->GetMyGrid(),false);
				ChosenAbilityAnim->CompletedCallback.BindUObject(this,&UPawnProcess_CalcAnim::AbilityCompleted);
				ChosenAbilityAnim->DoAnimation(CoReport,PawnInstance);
				LastReport.ProcessIndex = 1;
				// Report.HitInfoList.RemoveAt(i);
				return false;	
			}
		}
	}
	return true;
}

void UPawnProcess_CalcAnim::AbilityCompleted(TObjectPtr<AUnitAbilityAnim> Ability)
{
	UnitInstance->AttackDone();
	
	CastNum -= 1;
	if(CastNum <= 0)
	{
		PawnInstance->GetMyHUD()->ShowGameUI(true);
		PawnInstance->GetMyHUD()->GetGameUI()->PlayShowBattleUI();
	}
	CheckFlow(IDLE);
	UE_LOG(LogTemp,Log,TEXT("UPawnProcess_CalcAnim::AbilityCompleted"))
}

// void UPawnProcess_CalcAnim::AbilityCompletedEvent(TObjectPtr<AUnitAbilityAnim> Ability)
// {
// 	
// }

void UPawnProcess_CalcAnim::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	// UE_LOG(LogTemp,Log,TEXT("UPawnProcess_CalcAnim::EnterProcess"))
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
	Report = ChosenAbilityAnim->DoCalculation(TargetUnits,PawnInstance->GetMyGrid(),true);
	if(Report.HitInfoList.IsEmpty())
	{
		CastNum = 1;
		CastNum += Report.Cooperator != nullptr ? 1 : 0;	
	}
	else
	{
		CastNum = 1;
		for(const auto& one : Report.HitInfoList)
		{
			CastNum += one.Cooperator != nullptr ? 1 : 0;
		}
	}
	
	// ReportIndex = 0;
	//消耗AP
	UnitInstance->ConsumeAP(ChosenAbilityAnim->GetCost());
	//进入演出环节
	ChosenAbilityAnim->DoAnimation(Report,PawnInstance);
	PawnInstance->GetMyHUD()->ShowBattleInfoUI(true);
	PawnInstance->GetMyHUD()->ShowGameUI(true);
	PawnInstance->GetMyHUD()->GetGameUI()->PlayHideBattleUI();
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
	UE_LOG(LogTemp,Log,TEXT("UPawnProcess_CalcAnim::ExitProcess"))
	Super::ExitProcess();
	// ReportList.Empty();
	ChosenAbilityAnim->CompletedCallback.Unbind();
	ChosenAbilityAnim->CompletedEvent.Clear();
	PawnInstance->GetMyHUD()->ShowGameUI(false);
	PawnInstance->GetMyHUD()->ShowBattleInfoUI(false);
}
