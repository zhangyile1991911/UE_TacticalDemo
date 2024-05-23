// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitAction.h"
#include "UnitAction_Click.generated.h"

class UUnitAction_Move;
/**
 * 
 */
UCLASS()
class TBS20140401_API UUnitAction_Click : public UUnitAction
{
protected:
	GENERATED_BODY()
protected:
	UPROPERTY()
	TObjectPtr<UUnitAction_Move> MoveAction;
public:
	virtual void ExecuteAction(const FIntPoint& index) override;
};
