// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PawnProcess.generated.h"

class AMy_Pawn;
class AMyUnit;
/**
 * 
 */
UCLASS()
class TBS20140401_API UPawnProcess : public UObject
{
protected:
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<AMyUnit> UnitInstance;
	
	UPROPERTY()
	TObjectPtr<AMy_Pawn> PawnInstance;
public:
	virtual void EnterProcess(TObjectPtr<AMy_Pawn> Pawn,TObjectPtr<AMyUnit> Unit)
	{
		PawnInstance = Pawn;
		UnitInstance = Unit;
	};
	virtual void TickProcess(){};
	virtual void HandleDirectionInput(const FIntPoint& Index){};
	virtual void HandleCancelInput(){}
	virtual void HandleConfirmInput(){}
	virtual void ExitProcess(){};
};
