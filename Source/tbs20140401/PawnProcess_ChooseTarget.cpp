// Fill out your copyright notice in the Description page of Project Settings.


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
#include "EventCenter.h"
#include "GameUI_BattleInfo.h"
#include "UnitAbilityAnim.h"
#include "UnitInfoDetail.h"

void UPawnProcess_ChooseTarget::ShowTargetUnitBriefInfo(const FIntPoint& Index)
{
	const FTileData* TileData = PawnInstance->GetMyGrid()->GetTileDataByIndex(Index);
	if(TileData == nullptr)
	{
		UnitBriefInfoInstance->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	
	const TObjectPtr<AMyUnit> StandingUnit = TileData->UnitOnTile;
	bool bShow = true;
	
	if(StandingUnit == nullptr)
	{
		bool bIsSelf = Index == UnitInstance->GetTempDestinationGridIndex();
		bool bNeedToMove = UnitInstance->NeedToMove();
		if(bIsSelf && bNeedToMove)
		{
			UnitBriefInfoInstance->ShowTargetInfoAndTab(UnitInstance,-1);
			PawnInstance->GetEventCenter()->EventOfChosenUnit.Broadcast(UnitInstance->GetUniqueID());
		}
		else
		{
			bShow = false;	
		}
	}
	else
	{
		const bool bIsInArea = ArrayOfAbilityRange.Contains(Index);
		const bool bIsValid = ChosenAbilityPtr->IsValidTarget(TileData,PawnInstance->GetMyGrid());
		if(bIsValid&&bIsInArea)
		{
			// const bool bIsBackAttack = UBattleFunc::IsBackAttack(UnitInstance,StandingUnit);
			float HitPer = UBattleFunc::CalculateHitRate(UnitInstance,StandingUnit,PawnInstance->GetMyGrid(),bIsWrapAttack,bIsBackAttack);
			HitPer = FMath::Clamp(HitPer,0,100);
			//有效目标 确定 详情
			UnitBriefInfoInstance->ShowTarget(StandingUnit,HitPer);
		}
		else
		{//无效 详情
			if(TileData->UnitOnTile == UnitInstance)
			{
				bShow = false;
			}
			else
			{
				UnitBriefInfoInstance->ShowTargetInfoAndTab(StandingUnit,-1);	
			}
		}
		PawnInstance->GetEventCenter()->EventOfChosenUnit.Broadcast(StandingUnit->GetUniqueID());
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
		// UE_LOG(LogTemp,Log,TEXT("Target Location = %s Result = %hhd ScreenLocation = %s"),*WorldPosition.ToString(),Result,*ScreenLocation.ToString())
		UnitBriefInfoInstance->SetVisibility(ESlateVisibility::Visible);	
	}
	else
	{
		UnitBriefInfoInstance->SetVisibility(ESlateVisibility::Hidden);
		PawnInstance->GetEventCenter()->EventOfChosenUnit.Broadcast(0);
	}
}

void UPawnProcess_ChooseTarget::HideTargetUnitBriefInfo()
{
	if(UnitBriefInfoInstance != nullptr)
	{
		UnitBriefInfoInstance->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPawnProcess_ChooseTarget::ClearIndicatorRange()
{
	for(const auto& one : ArrayOfIndicatorRange)
	{
		PawnInstance->GetMyGrid()->RemoveStateFromTile(one,ETileState::IndicatorRange);	
	}
	ArrayOfIndicatorRange.Empty();
	PawnInstance->GetMyGrid()->RemoveStateFromTile(CurrentCursor,ETileState::IndicatorRange);
}

void UPawnProcess_ChooseTarget::SubscribeCameraActing()
{
	const FTileData* TileDataPtr = PawnInstance->GetMyGrid()->GetTileDataByIndex(CurrentCursor);
	if(BattleInfoInstance->IsVisible())
	{
		if(TileDataPtr->UnitOnTile)
		{
			BattleInfoInstance->UpdateWidgetPosition(TileDataPtr->UnitOnTile->GetActorLocation());	
		}
	}
	
	if(UnitBriefInfoInstance->IsVisible())
	{
		const FVector WorldPosition = TileDataPtr->UnitOnTile ? TileDataPtr->UnitOnTile->GetActorLocation() : TileDataPtr->Transform.GetLocation();
		UnitBriefInfoInstance->UpdateWidgetPosition(WorldPosition);
	}
	
}

void UPawnProcess_ChooseTarget::CheckBackAttack(TObjectPtr<AMyUnit> TempTarget)
{
	do
	{//背击判断
		if(ChosenAbilityPtr->IsArea())break;
		
		bIsBackAttack = false;
		if(TempTarget == nullptr)
		{
			BattleInfoInstance->HideBackAtkTips();
			break;
		}
		FIntPoint Delta = CurrentCursor - UnitInstance->GetTempDestinationGridIndex();
		if(FMathf::Abs(Delta.X) > 0 && FMathf::Abs(Delta.Y) > 0)break;
			
		//必须相邻 才会判断
		bIsBackAttack = UBattleFunc::IsBackAttack(UnitInstance,
			TempTarget,
			PawnInstance->GetMyGrid(),
			ChosenAbilityPtr->GetSkillData().AllowableDeviation);
		if(!bIsBackAttack)
		{
			BattleInfoInstance->HideBackAtkTips();
			break;
		}
		bool IsValid = ChosenAbilityPtr->IsValidUnit(TempTarget);
		if(IsValid)
		{
			BattleInfoInstance->ShowBackAtkTips(TempTarget);
		}
	}
	while (false);
}

void UPawnProcess_ChooseTarget::CheckCooperateAttack(TObjectPtr<AMyUnit> TempTarget)
{
	do
	{//夹击判断
		if(ChosenAbilityPtr->IsArea())break;
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
		BattleInfoInstance->ShowCooperatorTips(Cooperator);
	}
	while (false);
}

void UPawnProcess_ChooseTarget::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	UE_LOG(LogTemp,Log,TEXT("UPawnProcess_ChooseTarget::EnterProcess"))
	Super::EnterProcess(Pawn);
	UnitInstance = PawnInstance->GetMyCombatSystem()->GetFirstUnit();
	ChosenAbilityPtr = UnitInstance->GetChosenAbilityAnim();

	if(UnitInstance->NeedToMove())
		UnitInstance->ShowShadowUnit();

	if(ChosenAbilityPtr->IsIdle())
	{
		PawnInstance->SwitchToIdle();
		return;
	}
	
	// ChosenAbilityAnim = UnitInstance->GetChosenAbilityAnim();
	const FIntPoint StandIndex = UnitInstance->GetStandGridIndex();
	const int StandHeight = PawnInstance->GetMyGrid()->GetTileDataByIndex(StandIndex)->Height;
	CurrentCursor = PawnInstance->GetSelectedTile();
	
	//显示攻击范围
	ArrayOfAbilityRange = ChosenAbilityPtr->Range(UnitInstance->GetStandGridIndex());
	for(const FIntPoint& one : ArrayOfAbilityRange)
	{
		const FTileData* TileDataPtr = PawnInstance->GetMyGrid()->GetTileDataByIndex(one);
		if(TileDataPtr == nullptr)continue;
		if(!ChosenAbilityPtr->CheckDeviation(TileDataPtr->Height,StandHeight))
		{
			continue;
		}
		PawnInstance->GetMyGrid()->AddStateToTile(one,ETileState::AbilityRange);
	}

	auto Tmp = PawnInstance->GetMyHUD()->GetGameUI();
	UnitBriefInfoInstance = Tmp->GetUnitBriefInfo();
	BattleInfoInstance = PawnInstance->GetMyHUD()->GetBattleInfoUI();
	BattleInfoInstance->SetVisibility(ESlateVisibility::Visible);
	UnitDetailInfoPtr = Tmp->GetUnitDetailInfo();
	PawnInstance->OnCameraActing.AddDynamic(this,&UPawnProcess_ChooseTarget::SubscribeCameraActing);

	ShowTargetUnitBriefInfo(CurrentCursor);

	UnitDetailInfoPtr->SetVisibility(ESlateVisibility::Hidden);
	bIsTab = false;

	const TObjectPtr<AMyUnit> TempTarget = PawnInstance->GetMyGrid()->GetUnitOnTile(CurrentCursor);

	CheckBackAttack(TempTarget);
	CheckCooperateAttack(TempTarget);
	
	PawnInstance->GetEventCenter()->EventOfProcessChanged.Broadcast(FText::FromName(TEXT("ターゲットを選択")));
}

void UPawnProcess_ChooseTarget::TickProcess()
{
	Super::TickProcess();
}

void UPawnProcess_ChooseTarget::HandleDirectionInput(const FVector2D& Input)
{
	// Super::HandleDirectionInput(Input);
	if(bIsTab)return;
	
	FIntPoint next;
	// next.X = CurrentCursor.X + Input.Y;
	// next.Y = CurrentCursor.Y + Input.X;

	ECameraDirectType CameraDirect = PawnInstance->GetCurrentCameraDirect();
	switch (CameraDirect)
	{
	case ECameraDirectType::FORWARD:
		next.X = CurrentCursor.X + Input.Y;
		next.Y = CurrentCursor.Y + Input.X;
		break;
	case ECameraDirectType::BACKWARD:
		next.X = CurrentCursor.X - Input.Y;
		next.Y = CurrentCursor.Y - Input.X;
		break;
	case ECameraDirectType::LEFT:
		next.X = CurrentCursor.X - Input.X;
		next.Y = CurrentCursor.Y + Input.Y;
		break;
	case ECameraDirectType::RIGHT:
		next.X = CurrentCursor.X + Input.X;
		next.Y = CurrentCursor.Y - Input.Y;
		break;
	}

	// const FTileData* TileDataPtr = PawnInstance->GetMyGrid()->GetTileDataByIndex(next);
	// if(TileDataPtr == nullptr)
	// {
	// 	HideTargetUnitBriefInfo();
	// 	return;
	// }
	//
	// if(TileDataPtr->UnitOnTile == UnitInstance)
	// {
	// 	HideTargetUnitBriefInfo();
	// 	return;
	// }
	
	// if(!PawnInstance->GetMyGrid()->IsValidGridIndex(next))
	// {
	// 	ShowTargetUnitBriefInfo(next);
	// 	return;
	// }

	const FTileData* TileDataPtr = PawnInstance->GetMyGrid()->GetTileDataByIndex(next);
	if(TileDataPtr != nullptr)
	{
		PawnInstance->GetEventCenter()->EventOfChoseGrid.Broadcast(TileDataPtr);
	}
	//先清理掉上一次 指示器范围
	//前回のインジケーターをクリアする
	ClearIndicatorRange();
	
	
	PawnInstance->GetMyGrid()->RemoveStateFromTile(CurrentCursor,ETileState::Selected);
	//是否在施法范围
	if(!ArrayOfAbilityRange.Contains(next))
	{
		ShowTargetUnitBriefInfo(next);
		// PawnInstance->GetMyGrid()->RemoveStateFromTile(CurrentCursor,ETileState::Selected);
		PawnInstance->GetMyGrid()->AddStateToTile(next,ETileState::Selected);
		CurrentCursor = next;
		
		BattleInfoInstance->HideBackAtkTips();
		BattleInfoInstance->HideCooperatorTips();
		
		return;
	}
	
	if(ChosenAbilityPtr->IsArea())
	{
		ArrayOfIndicatorRange = ChosenAbilityPtr->Indicator(next);
		for(const auto& one : ArrayOfIndicatorRange)
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

	CheckBackAttack(TempTarget);
	CheckCooperateAttack(TempTarget);
	
	

	ShowTargetUnitBriefInfo(CurrentCursor);
}

void UPawnProcess_ChooseTarget::HandleCancelInput()
{
	Super::HandleCancelInput();
	if(bIsTab)
	{
		UnitDetailInfoPtr->SetVisibility(ESlateVisibility::Hidden);
		bIsTab = false;
	}
	else
	{
		PawnInstance->SwitchToCmdInput();	
	}
	
}

void UPawnProcess_ChooseTarget::HandleConfirmInput()
{
	Super::HandleConfirmInput();
	const FTileData* TileDataPtr = PawnInstance->GetMyGrid()->GetTileDataByIndex(CurrentCursor);
	
	if(!ArrayOfAbilityRange.Contains(CurrentCursor))return;

	if(!ChosenAbilityPtr->IsValidTarget(TileDataPtr,PawnInstance->GetMyGrid()))
	{
		return;	
	}
	
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
	
	for(const FIntPoint& one : ArrayOfAbilityRange)
	{
		PawnInstance->GetMyGrid()->RemoveStateFromTile(one,ETileState::AbilityRange);
	}
	
	ClearIndicatorRange();

	PawnInstance->GetMyGrid()->RemoveStateFromTile(CurrentCursor,ETileState::Selected);
	
	// PawnInstance->GetMyGrid()->RemoveStateFromTile(CurrentCursor,ETileState::Selected);
	HideTargetUnitBriefInfo();
	UnitBriefInfoInstance = nullptr;
		

	if(BattleInfoInstance != nullptr)
	{
		BattleInfoInstance->HideBackAtkTips();
		BattleInfoInstance->HideCooperatorTips();
		BattleInfoInstance->SetVisibility(ESlateVisibility::Hidden);
		BattleInfoInstance = nullptr;
	}
	if(UnitDetailInfoPtr != nullptr)
	{
		UnitDetailInfoPtr->SetVisibility(ESlateVisibility::Hidden);	
		UnitDetailInfoPtr = nullptr;
	}
	
	bIsTab = false;
	
	PawnInstance->OnCameraActing.RemoveDynamic(this,&UPawnProcess_ChooseTarget::SubscribeCameraActing);
}

void UPawnProcess_ChooseTarget::HandleLeftInput()
{
	if(!bIsTab)
		PawnInstance->CameraControlLeft();
}

void UPawnProcess_ChooseTarget::HandleRightInput()
{
	if(!bIsTab)
		PawnInstance->CameraControlRight();
}

void UPawnProcess_ChooseTarget::HandleZooming(float Val)
{
	if(!bIsTab)
		PawnInstance->CameraControlZooming(Val);
}

void UPawnProcess_ChooseTarget::HandleTabInput()
{
	if(!bIsTab)
	{
		do
		{
			if(CurrentCursor == UnitInstance->GetTempDestinationGridIndex())
			{
				auto Team = PawnInstance->GetMyCombatSystem()->GetOneSideTeam(UnitInstance->GetUnitSide());
				UnitDetailInfoPtr->ShowUnitTeamInfo(Team,UnitInstance);
				break;
			}

			const FTileData* TileDataPtr = PawnInstance->GetMyGrid()->GetTileDataByIndex(CurrentCursor);
			if(TileDataPtr == nullptr)return;
		
			if(TileDataPtr->UnitOnTile == nullptr)return;
			if(TileDataPtr->UnitOnTile == UnitInstance)return;
			auto Team = PawnInstance->GetMyCombatSystem()->GetOneSideTeam(TileDataPtr->UnitOnTile->GetUnitSide());
			UnitDetailInfoPtr->ShowUnitTeamInfo(Team,TileDataPtr->UnitOnTile);
			
		}while(false);
			
		
		
		bIsTab = true;
	}
}
