// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnProcess.h"
#include "PawnProcess_Story.generated.h"

class UBottomActionBar;
/**
 * 
 */
UCLASS()
class TBS20140401_API UPawnProcess_Story : public UPawnProcess
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	TObjectPtr<UBottomActionBar> UnitBottomActionBar;
public:
	virtual void EnterProcess(TObjectPtr<AMy_Pawn> Pawn) override;
	virtual void HandleConfirmInput() override;
	virtual void ExitProcess()override;
};
