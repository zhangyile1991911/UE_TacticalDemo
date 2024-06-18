// Fill out your copyright notice in the Description page of Project Settings.


#include "UPawnProcess_Normal.h"

#include "BottomActionBar.h"
#include "MyUnit.h"
#include "Grid.h"
#include "MyCombatSystem.h"
#include "My_Pawn.h"
#include "MyHUD.h"
#include "UGameUI_UnitBreifInfo.h"
#include "UnitInfoDetail.h"
#include "UnitPathComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

void UUPawnProcess_Normal::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	Super::EnterProcess(Pawn);

	// RelatedEnemies.Empty();
	auto MyBattleSystem = PawnInstance->GetMyCombatSystem();
	UnitInstance = MyBattleSystem->GetFirstUnit();
	
	ShowUnitWalkableRange();

	CurrentCursor = PawnInstance->GetSelectedTile();
	UE_LOG(LogTemp,Log,TEXT(" EnterProcess CurrentCursor (%d,%d)"),CurrentCursor.X,CurrentCursor.Y);
	//将临时目标设置为当前目标
	UnitInstance->SetTempDestinationGridIndex(CurrentCursor);
	
	ClearPathFinding();
	
	//查找路径
	if(CurrentCursor != UnitInstance->GetGridIndex())
	{
		FUnitPathFindingCompleted Completed;
        Completed.BindUObject(this,&UUPawnProcess_Normal::ShowWalkPath);
		UnitInstance->GetPathComponent()->UnitFindPathAsync(CurrentCursor,Completed);
	}
	
	//展示UI信息
	auto Tmp = PawnInstance->GetMyHUD()->GetGameUI();
	UnitBriefInfoPtr = Tmp->GetUnitBriefInfo();
	ShowTargetUnitBriefInfo(CurrentCursor);
	UnitDetailInfoPtr = Tmp->GetUnitDetailInfo();
	

	UnitInstance->HideShadowUnit();
	//将 摄像机 移动到 当前单位
	PawnInstance->LookAtUnit(UnitInstance);
	PawnInstance->GetMyGrid()->AddStateToTile(CurrentCursor,ETileState::Selected);

	Calucating = 0;
	CheckDangerousRange();
	CheckDangerousLine();
}

void UUPawnProcess_Normal::TickProcess()
{
	Super::TickProcess();
}

void UUPawnProcess_Normal::HandleDirectionInput(const FVector2D& Input)
{
	Super::HandleDirectionInput(Input);

	FIntPoint  Previous = CurrentCursor;
	FIntPoint Next;
	Next.X = CurrentCursor.X + Input.Y;
	Next.Y = CurrentCursor.Y + Input.X;
	
	if(PawnInstance->GetMyGrid()->IsValidGridIndex(Next))
	{
		PawnInstance->UpdateTileStatusByIndex(Next,ETileState::Selected);
		CurrentCursor = Next;
		UE_LOG(LogTemp,Log,TEXT(" HandleDirectionInput CurrentCursor (%d,%d)"),CurrentCursor.X,CurrentCursor.Y);
	}

	
	ClearPathFinding();
	
	if(UnitInstance->GetPathComponent()->IsMoveInReachableTiles(CurrentCursor))
	{//今選んだ目的地は行けるなら　路線を計算する
		FUnitPathFindingCompleted Completed;
		Completed.BindUObject(this,&UUPawnProcess_Normal::ShowWalkPath);
		UnitInstance->GetPathComponent()->UnitFindPathAsync(CurrentCursor,Completed);

		//更新威胁格子
		CheckMoveToDangerousRange(Previous,CurrentCursor);
	}
	
	ShowTargetUnitBriefInfo(CurrentCursor);
	
}

void UUPawnProcess_Normal::HandleCancelInput()
{
	Super::HandleCancelInput();
	UE_LOG(LogTemp,Log,TEXT("UUPawnProcess_Normal::HandleCancelInput()"))
	
	ClearPathFinding();
	UnitInstance->ResetTempDestinationGridIndex();
	PawnInstance->UpdateTileStatusByIndex(UnitInstance->GetGridIndex(),ETileState::Selected);
	
	PawnInstance->SwitchToIdle();
}

void UUPawnProcess_Normal::HandleConfirmInput()
{
	Super::HandleConfirmInput();
	
	if(UnitInstance->GetPathComponent()->IsMoveInReachableTiles(CurrentCursor))
	{
		//将影子放到目的地上(キャラのスタンドインを目的に置いとく)
		if(CurrentCursor != UnitInstance->GetGridIndex())
		{
			const auto TileData = PawnInstance->GetMyGrid()->GetTileDataByIndex(CurrentCursor);
			if(TileData == nullptr)
			{
				UE_LOG(LogTemp,Log,TEXT("CurrentCursor(%d,%d)"),CurrentCursor.X,CurrentCursor.Y);
				return;
			}
			if(TileData->UnitOnTile != nullptr)
			{
				return;
			}
			UnitInstance->MoveShadowOnTile(TileData->Transform.GetLocation());
			UnitInstance->SetTempDestinationGridIndex(CurrentCursor);
			PawnInstance->LookAtGrid(CurrentCursor);
		}
		PawnInstance->SwitchToCmdInput();
	}
		
}

void UUPawnProcess_Normal::HandleLeftInput()
{
	
}

void UUPawnProcess_Normal::HandleRightInput()
{
	
}

void UUPawnProcess_Normal::HandleTabInput()
{
	const auto TileDataPtr = PawnInstance->GetMyGrid()->GetTileDataByIndex(CurrentCursor);
	if(TileDataPtr == nullptr)return;
	if(TileDataPtr->UnitOnTile == nullptr)return;
	auto Team = PawnInstance->GetMyCombatSystem()->GetOneSideTeam(TileDataPtr->UnitOnTile->GetUnitSide());
	UnitDetailInfoPtr->ShowUnitTeamInfo(Team,TileDataPtr->UnitOnTile);
}


void UUPawnProcess_Normal::ExitProcess()
{
	UE_LOG(LogTemp,Log,TEXT("UUPawnProcess_Normal::ExitProcess"))
	Super::ExitProcess();
	ClearPathFinding();
	ClearWalkableTiles();
	ClearDangerousTiles();
	PawnInstance->RemoveTileStateByIndex(CurrentCursor,ETileState::Selected);
	UnitBriefInfoPtr->SetVisibility(ESlateVisibility::Hidden);
	ThreatenEnemies.Empty();
	// UnitInstance->HideShadowUnit();
	PawnInstance->GetMyCombatSystem()->HideUnitThreaten();
}

void UUPawnProcess_Normal::ClearPathFinding()
{
	for(const FIntPoint& one : PreviousPathFinding)
	{
		PawnInstance->GetMyGrid()->RemoveStateFromTile(one,ETileState::PathFinding);
	}
	PreviousPathFinding.Empty();
}

void UUPawnProcess_Normal::ClearWalkableTiles()
{
	for(const FIntPoint& one : UnitInstance->GetPathComponent()->GetReachableTiles())
	{
		PawnInstance->GetMyGrid()->RemoveStateFromTile(one,ETileState::Reachable);
	}
}

void UUPawnProcess_Normal::ShowWalkPath(TArray<FIntPoint> Path)
{
	ClearPathFinding();
	PreviousPathFinding = MoveTemp(Path);
	for(const FIntPoint& one : PreviousPathFinding)
	{
		PawnInstance->GetMyGrid()->AddStateToTile(one,ETileState::PathFinding);
	}
	UnitInstance->SaveWalkPath(PreviousPathFinding);
}

void UUPawnProcess_Normal::ShowTargetUnitBriefInfo(FIntPoint Index)
{
	bool bShow = true;
	
	const FTileData* TileData = PawnInstance->GetMyGrid()->GetTileDataByIndex(Index);
	TObjectPtr<AMyUnit> StandingUnit = TileData->UnitOnTile;
	if(StandingUnit != nullptr)
	{
		if(StandingUnit->GetGridIndex() == UnitInstance->GetGridIndex())
		{//目标是自己 只显示详情
			UnitBriefInfoPtr->ShowSelfCmd(UnitInstance);
		}
		else
		{//友军 敌人
			UnitBriefInfoPtr->ShowTargetInfoAndConfirmAndTab(StandingUnit,FText::FromName(TEXT("演习")),FText::FromName(TEXT("详情")));
		}
	}
	else
	{
		bool IsContain = UnitInstance->GetWalkableTiles().Contains(Index);
		if(IsContain)
		{
			UnitBriefInfoPtr->ShowConfirmCmd(FText::FromName(TEXT("移动")));
		}
		else
		{
			bShow = false;
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
    		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(UnitBriefInfoPtr->Slot);
    		CanvasSlot->SetPosition(ScreenLocation);
    	}
    	// UE_LOG(LogTemp,Log,TEXT("Target Location = %s Result = %hhd ScreenLocation = %s"),*WorldPosition.ToString(),Result,*ScreenLocation.ToString())
		UnitBriefInfoPtr->SetVisibility(ESlateVisibility::Visible);	
	}
	else
	{
		UnitBriefInfoPtr->SetVisibility(ESlateVisibility::Hidden);	
	}
	
	
}

void UUPawnProcess_Normal::HideTargetUnitBriefInfo()
{
	UnitBriefInfoPtr->SetVisibility(ESlateVisibility::Hidden);
}

void UUPawnProcess_Normal::CheckMoveToDangerousRange(const FIntPoint& Previous,const FIntPoint& Current)
{
	if(Previous == Current)return;
	//今回の移動は敵の移動範囲を踏むと　敵の移動範囲をやり直す
	for(int i = 0;i < ThreatenEnemies.Num();i++)
	{
		const auto EnemyPtr = ThreatenEnemies[i];
		const bool bIsMoveIn = EnemyPtr->GetPathComponent()->IsMoveInReachableTiles(Current);
		const bool bIsPrevMoveIn = EnemyPtr->GetPathComponent()->IsMoveInReachableTiles(Previous);
		if(bIsMoveIn)
		{
			Calucating++;
			FUnitWalkRangeCompleted Cal;
			Cal.BindUObject(this,&UUPawnProcess_Normal::WaitCalculating);
			EnemyPtr->GetPathComponent()->UnitWalkablePathAsync(Cal);
		}
		else if(bIsPrevMoveIn)
		{
			Calucating++;
			FUnitWalkRangeCompleted Cal;
			Cal.BindUObject(this,&UUPawnProcess_Normal::WaitCalculating);
			EnemyPtr->GetPathComponent()->UnitWalkablePathAsync(Cal);
		}
	}
	if(Calucating <= 0)CheckDangerousLine();
}

void UUPawnProcess_Normal::WaitCalculating()
{
	Calucating--;
	if(Calucating <= 0)
	{
		CheckDangerousRange();
		CheckDangerousLine();
	}
}

void UUPawnProcess_Normal::ClearDangerousTiles()
{
	for(const auto& one : DangerousTiles)
	{
		PawnInstance->GetMyGrid()->RemoveStateFromTile(one,ETileState::DangerousRange);
	}
	DangerousTiles.Empty();
}

void UUPawnProcess_Normal::CheckDangerousRange()
{
	ClearDangerousTiles();
	
	PawnInstance->GetMyCombatSystem()->HideUnitThreaten();
	
	ThreatenEnemies = PawnInstance->GetMyCombatSystem()->GetThreatenEnemies(UnitInstance);
	
	for(int i = 0;i < ThreatenEnemies.Num();i++)
	{
		const auto Enemy = ThreatenEnemies[i];
		
		const auto& r = Enemy->GetPathComponent()->GetReachableTiles();
		DangerousTiles.Append(r.Intersect(UnitInstance->GetPathComponent()->GetReachableTiles()));	
		
		const auto& a = Enemy->GetPathComponent()->GetAssaultRangeTiles();
		DangerousTiles.Append(a.Intersect(UnitInstance->GetPathComponent()->GetReachableTiles()));	
	}

	for(const auto& one : DangerousTiles)
	{
		PawnInstance->GetMyGrid()->AddStateToTile(one,ETileState::DangerousRange);
	}
}

void UUPawnProcess_Normal::CheckDangerousLine()
{
	auto TileDataPtr = PawnInstance->GetMyGrid()->GetTileDataByIndex(CurrentCursor);
	TArray<FThreatenInfo> Infos;
	for(int i = 0;i < ThreatenEnemies.Num();i++)
	{
		const auto Enemy = ThreatenEnemies[i];
		
		const auto& r = Enemy->GetPathComponent()->GetReachableTiles();
		if(r.Contains(CurrentCursor))
		{
			Infos.Add(FThreatenInfo{Enemy->GetActorLocation(),TileDataPtr->Transform.GetLocation()});
			continue;
		}
		
		const auto& a = Enemy->GetPathComponent()->GetAssaultRangeTiles();
		if(a.Contains(CurrentCursor))
		{
			Infos.Add(FThreatenInfo{Enemy->GetActorLocation(),TileDataPtr->Transform.GetLocation()});
		}
	}
	if(!Infos.IsEmpty())
		PawnInstance->GetMyCombatSystem()->ShowUnitThreaten(Infos);
	else
		PawnInstance->GetMyCombatSystem()->HideUnitThreaten();
}

void UUPawnProcess_Normal::ShowUnitWalkableRange()
{
	UnitInstance->GetPathComponent()->UnitWalkablePath();
	const auto& UnitWalkableTiles = UnitInstance->GetPathComponent()->GetTurnReachableTiles();
	for(const FIntPoint& one : UnitWalkableTiles)
	{
		PawnInstance->GetMyGrid()->AddStateToTile(one,ETileState::Reachable);
	}
	// Completed.BindUObject(this,&UUPawnProcess_Normal::ShowWalkableTiles);
}
