// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnProcess.h"
#include "PawnProcess_FinishTurn.generated.h"

class UGameSystemPanel;
class UMyGameInstance;
/**
 * 
 */
UCLASS()
class TBS20140401_API UPawnProcess_FinishTurn : public UPawnProcess
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	TObjectPtr<UGameSystemPanel> GameSystemPanel;

	UPROPERTY()
	UMyGameInstance* GameInstance;
	
	void ClearCurrentStage();
	UFUNCTION()
	void OnLevelUnloaded();
protected:
	virtual void EnterProcess(TObjectPtr<AMy_Pawn> Pawn) override;
	virtual void ExitProcess()override;
	virtual void HandleConfirmInput() override;
};
