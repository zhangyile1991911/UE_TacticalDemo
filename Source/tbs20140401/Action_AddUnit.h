// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyAction.h"
#include "Action_AddUnit.generated.h"

enum class EUnitType : uint8;
/**
 * 
 */
UCLASS()
class TBS20140401_API AAction_AddUnit : public AMyAction
{
	GENERATED_BODY()
protected:
	EUnitType UnitType;
	bool IsAddingUnit;
protected:
	virtual void ExecuteAction(const FIntPoint& index) override;
};
