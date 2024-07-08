// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyAction.h"
#include "Action_RotateUnit.generated.h"

UCLASS()
class TBS20140401_API AAction_RotateUnit : public AMyAction
{
	GENERATED_BODY()

	AAction_RotateUnit();
protected:
	virtual void ExecuteAction(const FIntPoint& index) override;
};
