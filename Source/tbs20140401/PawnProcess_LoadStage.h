// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnProcess.h"
#include "PawnProcess_LoadStage.generated.h"

class UGameSystemPanel;
/**
 * 
 */
UCLASS()
class TBS20140401_API UPawnProcess_LoadStage : public UPawnProcess
{
	GENERATED_BODY()
	bool bIsLoaded;

	UPROPERTY()
	TObjectPtr<UGameSystemPanel> GameSystemPanel;
public:
	virtual void HandleConfirmInput() override;
	virtual void EnterProcess(TObjectPtr<AMy_Pawn> Pawn) override;
	virtual void TickProcess() override;
	virtual void ExitProcess()override;
};
