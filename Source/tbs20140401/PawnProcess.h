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

public:
	virtual void BeginDestroy() override;

protected:
	UPROPERTY()
	TObjectPtr<AMyUnit> UnitInstance;
	
	UPROPERTY()
	TObjectPtr<AMy_Pawn> PawnInstance;
public:
	virtual void EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
	{
		PawnInstance = Pawn;
	};
	virtual void TickProcess(){};
	virtual void HandleDirectionInput(const FVector2D& Input){};
	virtual void HandleCancelInput(){}
	virtual void HandleConfirmInput(){}
	virtual void HandleTabInput(){}
	virtual void HandleSpaceInput(){}
	virtual void HandleLeftInput(){}
	virtual void HandleRightInput(){}
	virtual void HandleZooming(float){}
	virtual void ExitProcess(){}
};
