﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_ChooseTarget.h"

#include "My_Pawn.h"
#include "MyCombatSystem.h"
#include "MyUnit.h"
#include "Grid.h"
#include "MyGridPathfinding.h"
#include "UGameUI_UnitBreifInfo.h"
#include "MyHUD.h"
#include "BottomActionBar.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "BattleFunc.h"
#include "GameUI_BattleInfo.h"
#include "UnitAbilityAnim.h"

void UPawnProcess_ChooseTarget::ShowTargetUnitBriefInfo(const FIntPoint& Index)
{
	const FTileData* TileData = PawnInstance->GetMyGrid()->GetTileDataByIndex(Index);
	if(TileData == nullptr)
	{
		UnitBriefInfoInstance->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	
	TObjectPtr<AMyUnit> StandingUnit = TileData->UnitOnTile;
	bool bShow = true;
	bool IsValid = ChosenAbility->IsValidTarget(*TileData,PawnInstance->GetMyGrid());
	if(StandingUnit == nullptr)
	{
		bShow = false;
	}
	else
	{
		if(IsValid)
		{
			// const bool bIsBackAttack = UBattleFunc::IsBackAttack(UnitInstance,StandingUnit);
			float HitPer = UBattleFunc::CalculateHitRate(UnitInstance,StandingUnit,PawnInstance->GetMyGrid(),bIsWrapAttack,bIsBackAttack);
			HitPer = FMath::Clamp(HitPer,0,100);
			//有效目标 确定 详情
			UnitBriefInfoInstance->ShowTarget(UnitInstance,StandingUnit,HitPer,
				FText::FromName(TEXT("确定")),FText::FromName(TEXT("详情")));
		}
		else
		{//无效 详情
			UnitBriefInfoInstance->ShowTargetInfoAndTab(StandingUnit,0);
		}
	}
	if(bShow)
	{
		FVector2D ScreenLocation;
		const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

		const FVector WorldPosition = StandingUnit ? StandingUnit->GetActorLocation() : TileData->Transform.GetLocation();
		const bool Result = PlayerController->ProjectWorldLocationToScreen(WorldPosition,ScreenLocation,true);
		if(Result)
		{
			const float Scale = UWidgetLayoutLibrary::GetViewportScale(PlayerController);
			ScreenLocation /= Scale;
			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(UnitBriefInfoInstance->Slot);
			CanvasSlot->SetPosition(ScreenLocation);
		}
		UE_LOG(LogTemp,Log,TEXT("Target Location = %s Result = %hhd ScreenLocation = %s"),*WorldPosition.ToString(),Result,*ScreenLocation.ToString())
		UnitBriefInfoInstance->SetVisibility(ESlateVisibility::Visible);	
	}
	else
	{
		UnitBriefInfoInstance->SetVisibility(ESlateVisibility::Hidden);	
	}
}

void UPawnProcess_ChooseTarget::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	UE_LOG(LogTemp,Log,TEXT("UPawnProcess_ChooseTarget::EnterProcess"))
	Super::EnterProcess(Pawn);
	UnitInstance = PawnInstance->GetMyCombatSystem()->GetFirstUnit();
	ChosenAbility = UnitInstance->GetChosenAbilityAnim();

	if(UnitInstance->NeedToMove())
		UnitInstance->ShowShadowUnit();

	if(ChosenAbility->IsIdle())
	{
		PawnInstance->SwitchToIdle();
		return;
	}
	
	// ChosenAbilityAnim = UnitInstance->GetChosenAbilityAnim();

	CurrentCursor = PawnInstance->GetSelectedTile();
	//显示攻击范围
	AbilityRange = ChosenAbility->Range(CurrentCursor);
	for(const FIntPoint& one : AbilityRange)
	{
		PawnInstance->GetMyGrid()->AddStateToTile(one,ETileState::AbilityRange);
	}

	auto Tmp = PawnInstance->GetMyHUD()->GetGameUI();
	UnitBriefInfoInstance = Tmp->GetUnitBriefInfo();
	BattleInfoInstance = PawnInstance->GetMyHUD()->GetBattleInfoUI();
	
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

	if(!PawnInstance->GetMyGrid()->IsValidGridIndex(next))
	{
		ShowTargetUnitBriefInfo(next);
		return;
	}
	//先清理掉上一次 指示器范围
	for(const auto& one : IndicatorRange)
	{
		PawnInstance->GetMyGrid()->RemoveStateFromTile(one,ETileState::IndicatorRange);	
	}
	IndicatorRange.Empty();
	PawnInstance->GetMyGrid()->RemoveStateFromTile(CurrentCursor,ETileState::IndicatorRange);
	PawnInstance->GetMyGrid()->RemoveStateFromTile(CurrentCursor,ETileState::Selected);
	//是否在施法范围
	if(!AbilityRange.Contains(next))
	{
		ShowTargetUnitBriefInfo(next);
		// PawnInstance->GetMyGrid()->RemoveStateFromTile(CurrentCursor,ETileState::Selected);
		PawnInstance->GetMyGrid()->AddStateToTile(next,ETileState::Selected);
		CurrentCursor = next;
		return;
	}
	
	if(ChosenAbility->IsArea())
	{
		IndicatorRange = ChosenAbility->Indicator(next);
		for(const auto& one : IndicatorRange)
		{
			PawnInstance->GetMyGrid()->AddStateToTile(one,ETileState::IndicatorRange);	
		}
	}
	else
	{
		PawnInstance->GetMyGrid()->RemoveStateFromTile(CurrentCursor,ETileState::IndicatorRange);
		PawnInstance->GetMyGrid()->AddStateToTile(next,ETileState::IndicatorRange);
	}
	CurrentCursor = next;
		
	UnitInstance->RotateSelfByDestination(UnitInstance->GetTempDestinationGridIndex(),CurrentCursor);
	const TObjectPtr<AMyUnit> TempTarget = PawnInstance->GetMyGrid()->GetUnitOnTile(CurrentCursor);
	do
	{//背击判断
		if(ChosenAbility->IsArea())break;
		
		bIsBackAttack = false;
		if(TempTarget == nullptr)
		{
			BattleInfoInstance->HideBackAtkTips();
			break;
		}
		FIntPoint Delta = CurrentCursor - UnitInstance->GetTempDestinationGridIndex();
		if(FMathf::Abs(Delta.X) > 0 && FMathf::Abs(Delta.Y) > 0)break;
			
		//必须相邻 才会判断
		bIsBackAttack = UBattleFunc::IsBackAttack(UnitInstance,TempTarget);
		if(!bIsBackAttack)
		{
			BattleInfoInstance->HideBackAtkTips();
			break;
		}
		bool IsValid = ChosenAbility->IsValidUnit(TempTarget);
		if(IsValid)
		{
			BattleInfoInstance->SetVisibility(ESlateVisibility::Visible);
			BattleInfoInstance->ShowBackAtkTips(TempTarget);
		}
	}
	while (false);
		
	do
	{//夹击判断
		if(ChosenAbility->IsArea())break;
		bIsWrapAttack = false;
		if(TempTarget == nullptr)
		{
			BattleInfoInstance->HideCooperatorTips();
			break;
		}
		auto Cooperator = UBattleFunc::HasWrapAttackUnit(UnitInstance,TempTarget,PawnInstance->GetMyGrid());
		bIsWrapAttack = Cooperator != nullptr;
		if(!bIsWrapAttack)
		{
			BattleInfoInstance->HideCooperatorTips();
			break;
		}
		BattleInfoInstance->SetVisibility(ESlateVisibility::Visible);
		BattleInfoInstance->ShowCooperatorTips(Cooperator);
	}
	while (false);

	ShowTargetUnitBriefInfo(CurrentCursor);
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

	
	if(!ChosenAbility->IsValidTarget(*TileData,PawnInstance->GetMyGrid()))return;
	
	UnitInstance->SetAbilityTargetGridIndex(CurrentCursor);
	if(UnitInstance->NeedToMove())
	{
		PawnInstance->SwitchToMove();	
	}
	else
	{
		PawnInstance->SwitchToCalcAnim();
	}
	UnitInstance->HideShadowUnit();
}

void UPawnProcess_ChooseTarget::ExitProcess()
{
	UE_LOG(LogTemp,Log,TEXT("UPawnProcess_ChooseTarget::ExitProcess"))
	Super::ExitProcess();
	
	for(const FIntPoint& one : AbilityRange)
	{
		PawnInstance->GetMyGrid()->RemoveStateFromTile(one,ETileState::AbilityRange);
	}

	for(const auto& one : IndicatorRange)
	{
		PawnInstance->GetMyGrid()->RemoveStateFromTile(one,ETileState::IndicatorRange);	
	}
	
	PawnInstance->GetMyGrid()->RemoveStateFromTile(CurrentCursor,ETileState::IndicatorRange);
	PawnInstance->GetMyGrid()->RemoveStateFromTile(CurrentCursor,ETileState::Selected);
	
	// PawnInstance->GetMyGrid()->RemoveStateFromTile(CurrentCursor,ETileState::Selected);
	if(UnitBriefInfoInstance != nullptr)
	{
		UnitBriefInfoInstance->SetVisibility(ESlateVisibility::Hidden);
		UnitBriefInfoInstance = nullptr;
	}
		

	if(BattleInfoInstance != nullptr)
	{
		BattleInfoInstance->HideBackAtkTips();
		BattleInfoInstance->SetVisibility(ESlateVisibility::Hidden);
		BattleInfoInstance = nullptr;
	}
		
	// PawnInstance->UpdateTileStatusByIndex(CurrentCursor,ETileState::Selected);
}