// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UnitAction.generated.h"

class AMy_PC;
/**
 * 
 */
UCLASS()
class TBS20140401_API UUnitAction : public UObject
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	TObjectPtr<AMy_PC> MyPC;
public:
	virtual void ExecuteAction(const FIntPoint& index);
};
