// Fill out your copyright notice in the Description page of Project Settings.


#include "UPawnProcess_Normal.h"

#include "BottomActionBar.h"
#include "EventCenter.h"
#include "MyUnit.h"
#include "Grid.h"
#include "MyCombatSystem.h"
#include "My_Pawn.h"
#include "MyHUD.h"
#include "PathPointInst.h"
#include "UGameUI_UnitBreifInfo.h"
#include "UnitInfoDetail.h"
#include "UnitPathComponent.h"

void UUPawnProcess_Normal::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	Super::EnterProcess(Pawn);

	OtherUnitRange.Reserve(64);
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
	PawnInstance->OnCameraActing.AddDynamic(this,&UUPawnProcess_Normal::SubscribeCamera);

	UnitInstance->HideShadowUnit();
	//将 摄像机 移动到 当前单位
	PawnInstance->LookAtUnit(UnitInstance);
	PawnInstance->GetMyGrid()->AddStateToTile(CurrentCursor,ETileState::Selected);

	Calucating = 0;
	CheckDangerousRange();
	CheckDangerousLine();

	NotifyCurrentSelected();

	PawnInstance->GetEventCenter()->EventOfProcessChanged.Broadcast(FText::FromName(TEXT("移動先を選択")));
	
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
	
	ECameraDirectType CameraDirect = PawnInstance->GetCurrentCameraDirect();
	switch (CameraDirect)
	{
	case ECameraDirectType::FORWARD:
		Next.X = CurrentCursor.X + Input.Y;
		Next.Y = CurrentCursor.Y + Input.X;
		break;
	case ECameraDirectType::BACKWARD:
		Next.X = CurrentCursor.X - Input.Y;
		Next.Y = CurrentCursor.Y - Input.X;
		break;
	case ECameraDirectType::LEFT:
		Next.X = CurrentCursor.X - Input.X;
		Next.Y = CurrentCursor.Y + Input.Y;
		break;
	case ECameraDirectType::RIGHT:
		Next.X = CurrentCursor.X + Input.X;
		Next.Y = CurrentCursor.Y - Input.Y;
		break;
	}

	PawnInstance->LookAtGrid(CurrentCursor);
	
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
	}
	else
	{
		PawnInstance->GetMyPathPointInst()->HidePathPoint();
	}
	//更新威胁格子
	CheckMoveToDangerousRange(Previous,CurrentCursor);

	ShowOtherUnitRange(Next);
	
	ShowTargetUnitBriefInfo(CurrentCursor);

	NotifyCurrentSelected();
}

void UUPawnProcess_Normal::HandleCancelInput()
{
	
	UE_LOG(LogTemp,Log,TEXT("UUPawnProcess_Normal::HandleCancelInput()"))
	if(!bIsTab)
	{
		ClearPathFinding();
		UnitInstance->ResetTempDestinationGridIndex();
		PawnInstance->UpdateTileStatusByIndex(UnitInstance->GetGridIndex(),ETileState::Selected);
	
		PawnInstance->SwitchToIdle();	
	}
	else
	{
		UnitDetailInfoPtr->HideUnitTeamInfo();
		bIsTab = false;
	}
	
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
	if(bIsTab)
	{
		UnitDetailInfoPtr->PreviousUnit();
	}
	else
	{
		PawnInstance->CameraControlLeft();
	}
}

void UUPawnProcess_Normal::HandleRightInput()
{
	if(bIsTab)
	{
		UnitDetailInfoPtr->NextUnit();
	}
	else
	{
		PawnInstance->CameraControlRight();
	}
}

void UUPawnProcess_Normal::HandleZooming(float Val)
{
	if(!bIsTab)
	{
		PawnInstance->CameraControlZooming(Val);
	}
}

void UUPawnProcess_Normal::HandleTabInput()
{
	if(!bIsTab)
	{
		const auto TileDataPtr = PawnInstance->GetMyGrid()->GetTileDataByIndex(CurrentCursor);
		if(TileDataPtr == nullptr)return;
		if(TileDataPtr->UnitOnTile == nullptr)return;
		auto Team = PawnInstance->GetMyCombatSystem()->GetOneSideTeam(TileDataPtr->UnitOnTile->GetUnitSide());
		UnitDetailInfoPtr->ShowUnitTeamInfo(Team,TileDataPtr->UnitOnTile);
		bIsTab = true;
	}
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
	UnitDetailInfoPtr->HideUnitTeamInfo();
	PawnInstance->GetMyPathPointInst()->HidePathPoint();
	PawnInstance->OnCameraActing.RemoveDynamic(this,&UUPawnProcess_Normal::SubscribeCamera);
}

void UUPawnProcess_Normal::ClearPathFinding()
{
	// for(const FIntPoint& one : PreviousPathFinding)
	// {
	// 	PawnInstance->GetMyGrid()->RemoveStateFromTile(one,ETileState::PathFinding);
	// }
	// PreviousPathFinding.Empty();
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
	// PreviousPathFinding = MoveTemp(Path);
	// for(const FIntPoint& one : PreviousPathFinding)
	// {
	// 	PawnInstance->GetMyGrid()->AddStateToTile(one,ETileState::PathFinding);
	// }
	UnitInstance->SaveWalkPath(Path);
	PawnInstance->GetMyPathPointInst()->ShowPathPoint(Path,PawnInstance->GetMyGrid());
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
			UnitBriefInfoPtr->ShowTargetInfoAndConfirmAndTab(StandingUnit);
		}
	}
	else
	{
		bool IsContain = UnitInstance->GetPathComponent()->IsMoveInReachableTiles(Index);
		if(IsContain)
		{
			UnitBriefInfoPtr->ShowMoveOnly(TileData->Transform.GetLocation());
		}
		else
		{
			bShow = false;
		}	
	}

	if(bShow)
	{
		UnitBriefInfoPtr->SetVisibility(ESlateVisibility::Visible);
		UpdateUnitDetailInfoPosition(Index);
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
	if(UnitInstance->GetPathComponent()->IsMoveInReachableTiles(Current))
	{
		//今回の移動は敵の移動範囲を踏むと　敵の移動範囲をやり直す
		for(int i = 0;i < ThreatenEnemies.Num();i++)
		{
			const auto EnemyPtr = ThreatenEnemies[i];
			const bool bIsMoveIn = EnemyPtr->GetPathComponent()->IsMoveInReachableTiles(Current);
			const bool bIsPrevMoveIn = EnemyPtr->GetPathComponent()->IsMoveInReachableTiles(Previous);
			if(bIsMoveIn || bIsPrevMoveIn)
			{
				Calucating++;
				FUnitWalkRangeCompleted Cal;
				Cal.BindUObject(this,&UUPawnProcess_Normal::WaitCalculating);
				EnemyPtr->GetPathComponent()->UnitWalkablePathAsync(UnitInstance->GetUniqueID(),Cal);
			}
		}
		bIsRestore = false;
	}
	else
	{
		if(bIsRestore)return;
		//当选择的格子走出了 可移动范围，那么威胁格子就还原成最初的范围
		ThreatenEnemies = PawnInstance->GetMyCombatSystem()->GetThreatenEnemies(UnitInstance);
	
		for(int i = 0;i < ThreatenEnemies.Num();i++)
		{
			const auto Enemy = ThreatenEnemies[i];
		
			const auto& r = Enemy->GetPathComponent()->GetTurnReachableTiles();
			DangerousTiles.Append(r.Intersect(UnitInstance->GetPathComponent()->GetReachableTiles()));	
		
			const auto& a = Enemy->GetPathComponent()->GetTurnAssaultRangeTiles();
			DangerousTiles.Append(a.Intersect(UnitInstance->GetPathComponent()->GetReachableTiles()));	
		}

		for(const auto& one : DangerousTiles)
		{
			PawnInstance->GetMyGrid()->AddStateToTile(one,ETileState::DangerousRange);
		}
		bIsRestore = true;
	}
	
	if(Calucating <= 0)
		CheckDangerousLine();
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

void UUPawnProcess_Normal::ShowOtherUnitRange(const FIntPoint& Point)
{
	ClearOtherUnitRange();
	const auto TileDataPtr = PawnInstance->GetMyGrid()->GetTileDataByIndex(Point);
	if(TileDataPtr == nullptr)return;
	if(TileDataPtr->UnitOnTile == nullptr)return;
	if(TileDataPtr->UnitOnTile == UnitInstance)return;

	const TSet<FIntPoint>& Reachable = TileDataPtr->UnitOnTile->GetPathComponent()->GetTurnReachableTiles();
	for (const auto& One : Reachable)
	{
		PawnInstance->GetMyGrid()->AddStateToTile(One,ETileState::OtherUnitWalkRange);
		OtherUnitRange.Add(One);
	}
	
	bool Friend = TileDataPtr->UnitOnTile->IsFriend(UnitInstance->GetUnitSide());
	if(Friend == false)
	{
		const TSet<FIntPoint>& Assault = TileDataPtr->UnitOnTile->GetPathComponent()->GetTurnAssaultRangeTiles();
		for (const auto& One : Assault)
		{
			PawnInstance->GetMyGrid()->AddStateToTile(One,ETileState::OtherUnitAssaultRange);
			OtherUnitRange.Add(One);		
		}
	}
}

void UUPawnProcess_Normal::ClearOtherUnitRange()
{
	for(const auto& One : OtherUnitRange)
	{
		PawnInstance->GetMyGrid()->RemoveStateFromTile(One,ETileState::OtherUnitAssaultRange);
		PawnInstance->GetMyGrid()->RemoveStateFromTile(One,ETileState::OtherUnitWalkRange);
	}
	OtherUnitRange.Empty();
}

void UUPawnProcess_Normal::SubscribeCamera()
{
	UpdateUnitDetailInfoPosition(CurrentCursor);
}

void UUPawnProcess_Normal::UpdateUnitDetailInfoPosition(const FIntPoint& Point)
{
	
	const FTileData* TileDataPtr = PawnInstance->GetMyGrid()->GetTileDataByIndex(Point);
	if(TileDataPtr == nullptr)return;
	if(!UnitBriefInfoPtr->IsVisible())return;
	
	const FVector WorldPosition = TileDataPtr->UnitOnTile ? TileDataPtr->UnitOnTile->GetActorLocation() : TileDataPtr->Transform.GetLocation();
	UnitBriefInfoPtr->UpdateWidgetPosition(WorldPosition);
}

void UUPawnProcess_Normal::NotifyCurrentSelected()
{
	const FTileData* TileDataPtr = PawnInstance->GetMyGrid()->GetTileDataByIndex(CurrentCursor);
	if(TileDataPtr != nullptr)
	{
		PawnInstance->GetEventCenter()->EventOfChoseGrid.Broadcast(TileDataPtr);
	}
	if(TileDataPtr->UnitOnTile != nullptr)
	{
		PawnInstance->GetEventCenter()->EventOfChosenUnit.Broadcast(TileDataPtr->UnitOnTile->GetUniqueID());
	}
	else
	{
		PawnInstance->GetEventCenter()->EventOfChosenUnit.Broadcast(0);
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
{//展示受到威胁的格子
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
	if(!UnitInstance->GetPathComponent()->IsMoveInReachableTiles(CurrentCursor))
	{
		PawnInstance->GetMyCombatSystem()->HideUnitThreaten();
		return;
	}
	auto TileDataPtr = PawnInstance->GetMyGrid()->GetTileDataByIndex(CurrentCursor);
	TArray<FThreatenInfo> Infos;
	for(int i = 0;i < ThreatenEnemies.Num();i++)
	{
		const auto Enemy = ThreatenEnemies[i];
		
		const auto& r = Enemy->GetPathComponent()->GetTurnReachableTiles();
		if(r.Contains(CurrentCursor))
		{
			Infos.Add(FThreatenInfo{Enemy->GetActorLocation(),TileDataPtr->Transform.GetLocation()});
			continue;
		}
		
		const auto& a = Enemy->GetPathComponent()->GetTurnAssaultRangeTiles();
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
	UnitInstance->GetPathComponent()->UnitWalkablePath(UnitInstance->GetUniqueID());
	const auto& UnitWalkableTiles = UnitInstance->GetPathComponent()->GetTurnReachableTiles();
	for(const FIntPoint& one : UnitWalkableTiles)
	{
		PawnInstance->GetMyGrid()->AddStateToTile(one,ETileState::Reachable);
	}
	// Completed.BindUObject(this,&UUPawnProcess_Normal::ShowWalkableTiles);
}
