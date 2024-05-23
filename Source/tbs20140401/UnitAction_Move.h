// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitAction.h"
#include "UnitAction_Move.generated.h"

/**
 * 
 */
UCLASS()
class TBS20140401_API UUnitAction_Move : public UUnitAction
{
	GENERATED_BODY()
protected:
	
public:
	virtual void ExecuteAction(const FIntPoint& index) override;
};
