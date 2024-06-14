// Fill out your copyright notice in the Description page of Project Settings.


#include "UPawnProcess_Normal.h"

#include "BottomActionBar.h"
#include "MyUnit.h"
#include "Grid.h"
#include "MyCombatSystem.h"
#include "My_Pawn.h"
#include "MyHUD.h"
#include "UGameUI_UnitBreifInfo.h"
#include "UnitPathComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

void UUPawnProcess_Normal::abeee(TSet<FIntPoint> range)
{
	for(auto& one : range)
	{
		PawnInstance->GetMyGrid()->AddStateToTile(one,ETileState::Hovered);
	}
}

void UUPawnProcess_Normal::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	Super::EnterProcess(Pawn);

	RelatedEnemies.Empty();
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
	UnitBriefInfoInstance = Tmp->GetUnitBriefInfo();
	ShowTargetUnitBriefInfo(CurrentCursor);

	UnitInstance->HideShadowUnit();
	//将 摄像机 移动到 当前单位
	PawnInstance->LookAtUnit(UnitInstance);
	PawnInstance->GetMyGrid()->AddStateToTile(CurrentCursor,ETileState::Selected);

	Calucating = 0;
	CheckDangerousRange(true);
}

void UUPawnProcess_Normal::TickProcess()
{
	Super::TickProcess();
}

void UUPawnProcess_Normal::HandleDirectionInput(const FVector2D& Input)
{
	Super::HandleDirectionInput(Input);


	FIntPoint next;

	next.X = CurrentCursor.X + Input.Y;
	next.Y = CurrentCursor.Y + Input.X;
	
	if(PawnInstance->GetMyGrid()->IsValidGridIndex(next))
	{
		PawnInstance->UpdateTileStatusByIndex(next,ETileState::Selected);
		CurrentCursor = next;
		UE_LOG(LogTemp,Log,TEXT(" HandleDirectionInput CurrentCursor (%d,%d)"),CurrentCursor.X,CurrentCursor.Y);
	}

	
	ClearPathFinding();
	
	if(UnitInstance->GetPathComponent()->IsMoveInReachableTiles(CurrentCursor))
	{//今選んだ目的地は行けるなら　路線を計算する
		FUnitPathFindingCompleted Completed;
		Completed.BindUObject(this,&UUPawnProcess_Normal::ShowWalkPath);
		UnitInstance->GetPathComponent()->UnitFindPathAsync(CurrentCursor,Completed);

		//更新威胁格子
		CheckMoveToDangerousRange();
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
		PawnInstance->LookAtGrid(CurrentCursor);
		if(CurrentCursor != UnitInstance->GetGridIndex())
		{
			UnitInstance->MoveShadowOnTile(TileData->Transform.GetLocation());
			UnitInstance->SetTempDestinationGridIndex(CurrentCursor);
		}
		PawnInstance->SwitchToCmdInput();
	}
		
}

void UUPawnProcess_Normal::ExitProcess()
{
	UE_LOG(LogTemp,Log,TEXT("UUPawnProcess_Normal::ExitProcess"))
	Super::ExitProcess();
	ClearPathFinding();
	ClearWalkableTiles();
	PawnInstance->RemoveTileStateByIndex(CurrentCursor,ETileState::Selected);
	UnitBriefInfoInstance->SetVisibility(ESlateVisibility::Hidden);
	ThreatenEnemies.Empty();
	// UnitInstance->HideShadowUnit();
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
			UnitBriefInfoInstance->ShowSelfCmd(UnitInstance);
		}
		else
		{//友军 敌人
			UnitBriefInfoInstance->ShowTargetInfoAndConfirmAndTab(StandingUnit,FText::FromName(TEXT("演习")),FText::FromName(TEXT("详情")));
		}
	}
	else
	{
		bool IsContain = UnitInstance->GetWalkableTiles().Contains(Index);
		if(IsContain)
		{
			UnitBriefInfoInstance->ShowConfirmCmd(FText::FromName(TEXT("移动")));
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

void UUPawnProcess_Normal::HideTargetUnitBriefInfo()
{
	UnitBriefInfoInstance->SetVisibility(ESlateVisibility::Hidden);
}

void UUPawnProcess_Normal::CheckMoveToDangerousRange()
{
	TSet<uint32> AffectedEnemy;
	for(int i = 0;i < ThreatenEnemies.Num();i++)
	{
		const auto EnemyPtr = ThreatenEnemies[i];
		const bool bIsMoveIn = EnemyPtr->GetPathComponent()->IsMoveInReachableTiles(CurrentCursor);
		if(bIsMoveIn)
		{//今回の移動は影響した敵を記録する
			AffectedEnemy.Add(EnemyPtr->GetUniqueID());
		}
	}
	
	for(int i = 0;i < ThreatenEnemies.Num();i++)
	{
		const auto EnemyPtr = ThreatenEnemies[i];
		if(AffectedEnemy.Contains(EnemyPtr->GetUniqueID()))
		{
			Calucating++;
			FUnitWalkRangeCompleted Cal;
			Cal.BindUObject(this,&UUPawnProcess_Normal::WaitCalculating);
			EnemyPtr->GetPathComponent()->UnitWalkablePathAsync(Cal);
			if(!RelatedEnemies.Contains(EnemyPtr->GetUniqueID()))
				RelatedEnemies.Add(EnemyPtr->GetUniqueID(),EnemyPtr);
		}
		else if(RelatedEnemies.Contains(EnemyPtr->GetUniqueID()))
		{
			Calucating++;
			FUnitWalkRangeCompleted Cal;
			Cal.BindUObject(this,&UUPawnProcess_Normal::WaitCalculating);
			EnemyPtr->GetPathComponent()->UnitWalkablePathAsync(Cal);
			RelatedEnemies.Remove(EnemyPtr->GetUniqueID());
		}
	}
	
	
}

void UUPawnProcess_Normal::WaitCalculating()
{
	Calucating--;
	if(Calucating <= 0)
	{
		CheckDangerousRange(false);
	}
}

void UUPawnProcess_Normal::CheckDangerousRange(bool First)
{
	for(const auto& one : DangerousTiles)
	{
		PawnInstance->GetMyGrid()->RemoveStateFromTile(one,ETileState::DangerousRange);
	}
	DangerousTiles.Empty();
	
	ThreatenEnemies = PawnInstance->GetMyCombatSystem()->GetThreatenEnemies(UnitInstance);
	for(int i = 0;i < ThreatenEnemies.Num();i++)
	{
		const auto Enemy = ThreatenEnemies[i];
		if(First)
		{
			const auto& r = Enemy->GetPathComponent()->GetTurnReachableTiles();
			DangerousTiles.Append(r.Intersect(UnitInstance->GetPathComponent()->GetReachableTiles()));
		}
		else
		{
			const auto& r = Enemy->GetPathComponent()->GetReachableTiles();
			DangerousTiles.Append(r.Intersect(UnitInstance->GetPathComponent()->GetReachableTiles()));	
		}
		
		if(First)
		{
			const auto& a = Enemy->GetPathComponent()->GetTurnAssaultRangeTiles();
			DangerousTiles.Append(a.Intersect(UnitInstance->GetPathComponent()->GetReachableTiles()));
		}
		else
		{
			const auto& a = Enemy->GetPathComponent()->GetAssaultRangeTiles();
			DangerousTiles.Append(a.Intersect(UnitInstance->GetPathComponent()->GetReachableTiles()));	
		}
		
	}

	for(const auto& one : DangerousTiles)
	{
		PawnInstance->GetMyGrid()->AddStateToTile(one,ETileState::DangerousRange);
	}
	
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
