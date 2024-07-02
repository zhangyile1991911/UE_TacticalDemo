// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnProcess.h"
#include "PawnProcess_CMD.generated.h"

class UBottomActionBar;
class UCmdWidget;
class UUGameUI_UnitBriefInfo;
/**
 * 
 */
UCLASS()
class TBS20140401_API UPawnProcess_CMD : public UPawnProcess
{
	GENERATED_BODY()
protected:
	TObjectPtr<AMyUnit> UnitInstance;
	int CmdIndex;
	TObjectPtr<UBottomActionBar> BottomActionBarInstance;
	TObjectPtr<UCmdWidget> CmdWidgetInstance;

	UPROPERTY()
	TObjectPtr<UUGameUI_UnitBriefInfo> UnitBriefInfoPtr;

	TArray<FIntPoint> ArrayOfAbilityRange;

	void ShowBriefInfo();
	void ClearAbilityRange();
public:
	virtual void EnterProcess(TObjectPtr<AMy_Pawn> Pawn) override; 
	virtual void TickProcess()override;
	virtual void HandleDirectionInput(const FVector2D& Input)override;
	virtual void HandleCancelInput()override;
	virtual void HandleConfirmInput()override;
	virtual void HandleSpaceInput() override;
	virtual void ExitProcess()override;
};
