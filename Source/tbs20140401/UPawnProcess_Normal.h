// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGridPathfinding.h"
#include "PawnProcess.h"
#include "UPawnProcess_Normal.generated.h"

/**
 * 
 */
UCLASS()
class TBS20140401_API UUPawnProcess_Normal : public UPawnProcess
{
	GENERATED_BODY()
protected:
	//当前光标
	FIntPoint CurrentCursor;
	//记录寻路的路径
	TArray<FIntPoint> PreviousPathFinding;
	FPathFindingCompleted Completed;
	void ShowWalkableTiles(TArray<FIntPoint> tiles);
	//记录临时选择目的地
	FIntPoint TempTargetTile;
	void ClearPathFinding();
public:
	virtual void EnterProcess(TObjectPtr<AMy_Pawn> Pawn,TObjectPtr<AMyUnit> Unit) override;
	virtual void TickProcess() override;
	virtual void HandleDirectionInput(const FIntPoint& Index)override;
	virtual void HandleCancelInput()override;
	virtual void HandleConfirmInput() override;
	virtual void ExitProcess()override;
};
