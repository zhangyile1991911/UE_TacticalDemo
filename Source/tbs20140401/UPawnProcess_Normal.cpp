// Fill out your copyright notice in the Description page of Project Settings.


#include "UPawnProcess_Normal.h"

#include "BottomActionBar.h"
#include "MyUnit.h"
#include "Grid.h"
#include "MyCombatSystem.h"
#include "My_Pawn.h"
#include "MyHUD.h"
#include "UGameUI_UnitBreifInfo.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"


void UUPawnProcess_Normal::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	Super::EnterProcess(Pawn);

	// DoubleCheck = false;
	
	UnitInstance = PawnInstance->GetMyCombatSystem()->GetFirstUnit();
	
	TArray<FIntPoint> WalkableRange = PawnInstance->GetMyGridPathFinding()->UnitWalkablePath(
			UnitInstance->GetGridIndex(),
			UnitInstance->GetRuntimeProperty().Move,
			UnitInstance->UnitCanWalkTileType());
	UnitInstance->SetWalkableTile(WalkableRange);

	for(const FIntPoint& one : WalkableRange)
	{
		PawnInstance->GetMyGrid()->AddStateToTile(one,ETileState::Reachable);
	}
	
	Completed.BindUObject(this,&UUPawnProcess_Normal::ShowWalkableTiles);

	CurrentCursor = PawnInstance->GetSelectedTile();
	UE_LOG(LogTemp,Log,TEXT(" EnterProcess CurrentCursor (%d,%d)"),CurrentCursor.X,CurrentCursor.Y);
	PawnInstance->UpdateTileStatusByIndex(CurrentCursor,ETileState::Selected);
	ClearPathFinding();

	if(CurrentCursor != UnitInstance->GetGridIndex())
	{
		PawnInstance->GetMyGridPathFinding()->UnitFindPath(
			UnitInstance->GetGridIndex(),
			CurrentCursor,
			UnitInstance->UnitCanWalkTileType(),
			Completed);
	}

	auto Tmp = PawnInstance->GetMyHUD()->GetGameUI();
	UnitBriefInfoInstance = Tmp->GetUnitBriefInfo();
	
	// PawnInstance->GetMyGrid()->AddStateToTile(CurrentCursor,ETileState::Selected);
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
		// PawnInstance->GetMyGrid()->RemoveStateFromTile(CurrentCursor,ETileState::Selected);
		// PawnInstance->GetMyGrid()->AddStateToTile(next,ETileState::Selected);
		CurrentCursor = next;
		UE_LOG(LogTemp,Log,TEXT(" HandleDirectionInput CurrentCursor (%d,%d)"),CurrentCursor.X,CurrentCursor.Y);
	}
	
	ClearPathFinding();
	
	if(UnitInstance->IsInWalkableRange(CurrentCursor))
	{//今選んだ目的地は行けるなら　路線を計算する
		PawnInstance->GetMyGridPathFinding()->UnitFindPath(
			UnitInstance->GetGridIndex(),
			CurrentCursor,
			UnitInstance->UnitCanWalkTileType(),
			Completed);
	}

	TObjectPtr<AMyUnit> StandingUnit = PawnInstance->GetMyGrid()->GetUnitOnTile(CurrentCursor);
	if(StandingUnit)
	{//显示简略信息(簡略なメッセージを表示する)
		ShowTargetUnitBriefInfo(StandingUnit);
	}
	else
	{//隐藏简略信息(簡略なメッセージを非表示する)
		HideTargetUnitBriefInfo();
	}
	
}

void UUPawnProcess_Normal::HandleCancelInput()
{
	Super::HandleCancelInput();
	UE_LOG(LogTemp,Log,TEXT("UUPawnProcess_Normal::HandleCancelInput()"))
	
	ClearPathFinding();
	
	PawnInstance->UpdateTileStatusByIndex(UnitInstance->GetGridIndex(),ETileState::Selected);
	
}

void UUPawnProcess_Normal::HandleConfirmInput()
{
	Super::HandleConfirmInput();
	
	if(!UnitInstance->IsInWalkableRange(CurrentCursor))
	{
		if(PawnInstance->GetMyGrid()->TileGridHasUnit(CurrentCursor))
		{//进入指令界面(コマンドページへ変遷)
			
		}
	}
	else
	{
		//将影子放到目的地上(キャラのスタンドインを目的に置いとく)
		const auto TileData = PawnInstance->GetMyGrid()->GetTileDataByIndex(CurrentCursor);
		if(TileData == nullptr)
		{
			UE_LOG(LogTemp,Log,TEXT("CurrentCursor(%d,%d)"),CurrentCursor.X,CurrentCursor.Y);
			return;
		}
		UnitInstance->MoveShadowOnTile(TileData->Transform.GetLocation());

		PawnInstance->SwitchToCmdInput();
	}

	
}

void UUPawnProcess_Normal::ExitProcess()
{
	Super::ExitProcess();
	Completed.Unbind();
	ClearPathFinding();
	PawnInstance->RemoveTileStateByIndex(CurrentCursor,ETileState::Selected);
	// UnitInstance->HideShadowUnit();
}


void UUPawnProcess_Normal::ShowWalkableTiles(TArray<FIntPoint> tiles)
{
	for(const FIntPoint& one : tiles)
	{
		PawnInstance->GetMyGrid()->AddStateToTile(one,ETileState::PathFinding);
	}
	PreviousPathFinding = MoveTemp(tiles);
}

void UUPawnProcess_Normal::ClearPathFinding()
{
	for(const FIntPoint& one : PreviousPathFinding)
	{
		PawnInstance->GetMyGrid()->RemoveStateFromTile(one,ETileState::PathFinding);
	}
	PreviousPathFinding.Empty();
}

void UUPawnProcess_Normal::ShowTargetUnitBriefInfo(TObjectPtr<AMyUnit> TargetUint)
{
	UnitBriefInfoInstance->SetVisibility(ESlateVisibility::Visible);
	UnitBriefInfoInstance->RefreshUnitBriefInfo(UnitInstance,TargetUint);

	FVector2D ScreenLocation;
	const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	const bool Result = PlayerController->ProjectWorldLocationToScreen(TargetUint->GetActorLocation(),ScreenLocation,true);
	if(Result)
	{
		const float Scale = UWidgetLayoutLibrary::GetViewportScale(PlayerController);
		ScreenLocation /= Scale;
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(UnitBriefInfoInstance->Slot);
		CanvasSlot->SetPosition(ScreenLocation);
	}
	UE_LOG(LogTemp,Log,TEXT("Target Location = %s Result = %hhd ScreenLocation = %s"),*TargetUint->GetActorLocation().ToString(),Result,*ScreenLocation.ToString())
}

void UUPawnProcess_Normal::HideTargetUnitBriefInfo()
{
	UnitBriefInfoInstance->SetVisibility(ESlateVisibility::Hidden);
}
