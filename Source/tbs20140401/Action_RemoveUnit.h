// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyAction.h"
#include "Action_RemoveUnit.generated.h"

/**
 * 
 */
UCLASS()
class TBS20140401_API AAction_RemoveUnit : public AMyAction
{
	GENERATED_BODY()
protected:
	virtual void ExecuteAction(const FIntPoint& index) override;
};