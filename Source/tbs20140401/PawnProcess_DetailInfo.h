// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnProcess.h"
#include "PawnProcess_DetailInfo.generated.h"

class UUnitInfoDetail;
/**
 * 
 */
UCLASS()
class TBS20140401_API UPawnProcess_DetailInfo : public UPawnProcess
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UUnitInfoDetail> UnitDetailInfoPtr;
protected:
	virtual void EnterProcess(TObjectPtr<AMy_Pawn> Pawn) override;
	// virtual void HandleLeftInput()override;
	// virtual void HandleRightInput()override;
	virtual void HandleCancelInput()override;
	virtual void ExitProcess()override;
};
