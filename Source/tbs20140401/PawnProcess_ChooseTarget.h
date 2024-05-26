// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnProcess.h"
#include "PawnProcess_ChooseTarget.generated.h"

/**
 * 
 */
UCLASS()
class TBS20140401_API UPawnProcess_ChooseTarget : public UPawnProcess
{
	GENERATED_BODY()
protected:
	FIntPoint CurrentCursor;
	TArray<FIntPoint> AbilityRange;
public:
	virtual void EnterProcess(TObjectPtr<AMy_Pawn> Pawn)override;
	virtual void TickProcess()override;
	virtual void HandleDirectionInput(const FVector2D& Input)override;
	virtual void HandleCancelInput()override;
	virtual void HandleConfirmInput()override;
	virtual void ExitProcess()override;
};
