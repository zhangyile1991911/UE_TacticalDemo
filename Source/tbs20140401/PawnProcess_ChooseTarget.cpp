// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_ChooseTarget.h"

#include "My_Pawn.h"
#include "MyCombatSystem.h"
#include "MyUnit.h"
#include "Grid.h"
#include "MyGridPathfinding.h"
#include "UnitAbility.h"
#include "UGameUI_UnitBreifInfo.h"
#include "MyHUD.h"
#include "BottomActionBar.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "BattleFunc.h"

void UPawnProcess_ChooseTarget::ShowTargetUnitBriefInfo(const FIntPoint& Index)
{
	const FTileData* TileData = PawnInstance->GetMyGrid()->GetTileDataByIndex(Index);
	TObjectPtr<AMyUnit> StandingUnit = TileData->UnitOnTile;
	bool bShow = true;
	if(StandingUnit == nullptr)
	{
		bShow = false;
	}
	else
	{
		bool IsValid = ChosenAbility->IsValidTarget(*TileData);
		if(IsValid)
		{
			const bool bIsBackAttack = UBattleFunc::IsBackAttack(UnitInstance,StandingUnit);
			const bool bIsWrapAttack = UBattleFunc::HasWrapAttackUnit(UnitInstance,StandingUnit,PawnInstance->GetMyGrid());
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

	auto Tmp = PawnInstance->GetMyHUD()->GetGameUI();
	UnitBriefInfoInstance = Tmp->GetUnitBriefInfo();
	
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
		//所以说X正轴 角度为0
		UE_LOG(LogTemp,Log,TEXT("%f"),FVector(0,1,0).Rotation().Yaw)//90.000000
		UE_LOG(LogTemp,Log,TEXT("%f"),FVector(1,0,0).Rotation().Yaw)//0.000000
		UE_LOG(LogTemp,Log,TEXT("%f"),FVector(0,-1,0).Rotation().Yaw)// -90.000000
		UE_LOG(LogTemp,Log,TEXT("%f"),FVector(-1,0,0).Rotation().Yaw)//180.000000
		//当前的朝向和选择的目标夹角是否大于45度 
		// FVector Location = UnitInstance->GetActorLocation();
		// const FTileData* TargetTileData = PawnInstance->GetMyGrid()->GetTileDataByIndex(CurrentCursor);
		// FVector TargetLocation = TargetTileData->Transform.GetLocation();
		// float Degree = (TargetLocation - Location).GetSafeNormal().Rotation().Yaw;
		// if(Degree >= 45 && Degree <= -45)
		// {
		// 	UnitInstance->TurnShadowForward();
		// }
		// else if(Degree >= 90-45 && Degree <= 90+45)
		// {
		// 	UnitInstance->TurnShadowRight();
		// }
		// else if(Degree >= 180-45 && Degree <= 180+45)
		// {
		// 	UnitInstance->TurnShadowBack();
		// }
		// else if(Degree >= -90-45 && Degree <= -90+45)
		// {
		// 	UnitInstance->TurnShadowLeft();
		// }
		UnitInstance->RotateSelfByDestination(CurrentCursor,UnitInstance->GetTempDestinationGridIndex());
	}
	
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

	
	if(!ChosenAbility->IsValidTarget(*TileData))return;
	
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
	Super::ExitProcess();
	
	for(const FIntPoint& one : AbilityRange)
	{
		PawnInstance->GetMyGrid()->RemoveStateFromTile(one,ETileState::AbilityRange);
	}
	PawnInstance->GetMyGrid()->RemoveStateFromTile(CurrentCursor,ETileState::Selected);
	UnitBriefInfoInstance->SetVisibility(ESlateVisibility::Hidden);	
	// PawnInstance->UpdateTileStatusByIndex(CurrentCursor,ETileState::Selected);
}
