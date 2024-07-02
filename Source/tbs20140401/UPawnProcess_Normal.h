// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnProcess.h"
#include "UPawnProcess_Normal.generated.h"

class UUGameUI_UnitBriefInfo;
class UUnitInfoDetail;
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
	// TArray<FIntPoint> PreviousPathFinding;

	TArray<FIntPoint> OtherUnitRange;

	TArray<TObjectPtr<AMyUnit>> ThreatenEnemies;
	// TMap<uint32,TObjectPtr<AMyUnit>> RelatedEnemies;//影響をされた敵
	TSet<FIntPoint> DangerousTiles;
	UPROPERTY()
	TObjectPtr<UUGameUI_UnitBriefInfo> UnitBriefInfoPtr;
	UPROPERTY()
	TObjectPtr<UUnitInfoDetail> UnitDetailInfoPtr;

	int Calucating = 0;
	bool bIsTab = false;
	bool bIsRestore = false;
protected:
	void ClearPathFinding();
	void ClearWalkableTiles();
	void ClearDangerousTiles();
	void ShowWalkPath(TArray<FIntPoint> Path);
	void ShowTargetUnitBriefInfo(FIntPoint Index);
	void HideTargetUnitBriefInfo();
	void CheckDangerousRange();
	void CheckDangerousLine();
	void CheckMoveToDangerousRange(const FIntPoint& Previous,const FIntPoint& Current);
	void ShowUnitWalkableRange();
	void WaitCalculating();
	void ShowOtherUnitRange(const FIntPoint&);
	void ClearOtherUnitRange();
	UFUNCTION()
	void SubscribeCamera();
	void UpdateUnitDetailInfoPosition(const FIntPoint& Point);
	void NotifyCurrentSelected();
public:
	virtual void EnterProcess(TObjectPtr<AMy_Pawn> Pawn) override;
	virtual void TickProcess() override;
	virtual void HandleDirectionInput(const FVector2D& Input)override;
	virtual void HandleCancelInput()override;
	virtual void HandleConfirmInput() override;
	virtual void HandleLeftInput()override;
	virtual void HandleRightInput()override;
	virtual void HandleZooming(float Val)override;
	virtual void HandleTabInput()override;
	virtual void ExitProcess()override;
	
};
