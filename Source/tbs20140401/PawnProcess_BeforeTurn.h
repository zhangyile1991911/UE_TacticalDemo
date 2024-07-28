// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnProcess.h"
#include "PawnProcess_BeforeTurn.generated.h"

class UMyGameInstance;
/**
 * 
 */
UCLASS()
class TBS20140401_API UPawnProcess_BeforeTurn : public UPawnProcess
{
	GENERATED_BODY()

	UPROPERTY()
	UMyGameInstance* GameInstance;

	int ReadStoryNum = 0;
public:

protected:
	virtual void EnterProcess(TObjectPtr<AMy_Pawn> Pawn) override;
	virtual void TickProcess()override;
	virtual void HandleDirectionInput(const FVector2D& Input)override;
	virtual void HandleCancelInput()override;
	virtual void HandleConfirmInput()override;
	virtual void ExitProcess()override;
};
