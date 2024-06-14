﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	

	TArray<TObjectPtr<AMyUnit>> ThreatenEnemies;
	TMap<uint32,TObjectPtr<AMyUnit>> RelatedEnemies;//影響をされた敵
	TSet<FIntPoint> DangerousTiles;
	TObjectPtr<UUGameUI_UnitBriefInfo> UnitBriefInfoInstance;


	int Calucating = 0;
protected:
	void ClearPathFinding();
	void ClearWalkableTiles();
	void ShowWalkPath(TArray<FIntPoint> Path);
	void ShowTargetUnitBriefInfo(FIntPoint Index);
	void HideTargetUnitBriefInfo();
	void CheckDangerousRange(bool First);
	void CheckMoveToDangerousRange();
	void ShowUnitWalkableRange();
	void WaitCalculating();
public:
	virtual void EnterProcess(TObjectPtr<AMy_Pawn> Pawn) override;
	virtual void TickProcess() override;
	virtual void HandleDirectionInput(const FVector2D& Input)override;
	virtual void HandleCancelInput()override;
	virtual void HandleConfirmInput() override;
	virtual void ExitProcess()override;

	void abeee(TSet<FIntPoint> range);
};
