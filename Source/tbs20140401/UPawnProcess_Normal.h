// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGridPathfinding.h"
#include "PawnProcess.h"
#include "UPawnProcess_Normal.generated.h"

class UUGameUI_UnitBriefInfo;
/**
 * 
 */
UCLASS()
class TBS20140401_API UUPawnProcess_Normal : public UPawnProcess
{
	GENERATED_BODY()
protected:
	// bool DoubleCheck;
	//当前光标
	FIntPoint CurrentCursor;
	//记录寻路的路径
	TArray<FIntPoint> PreviousPathFinding;
	FPathFindingCompleted Completed;
	
	//记录临时选择目的地
	FIntPoint TempTargetTile;
	
	TObjectPtr<UUGameUI_UnitBriefInfo> UnitBriefInfoInstance;
protected:
	void ClearPathFinding();
	void ClearWalkableTiles();
	void ShowWalkableTiles(TArray<FIntPoint> tiles);
	void ShowTargetUnitBriefInfo(FIntPoint Index);
	void HideTargetUnitBriefInfo();
public:
	virtual void EnterProcess(TObjectPtr<AMy_Pawn> Pawn) override;
	virtual void TickProcess() override;
	virtual void HandleDirectionInput(const FVector2D& Input)override;
	virtual void HandleCancelInput()override;
	virtual void HandleConfirmInput() override;
	virtual void ExitProcess()override;
};
