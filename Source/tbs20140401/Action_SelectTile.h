// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyAction.h"
#include "Action_SelectTile.generated.h"

class AMyUnit;
/**
 * 
 */
UCLASS()
class TBS20140401_API AAction_SelectTile : public AMyAction
{
	GENERATED_BODY()
protected:
	FIntPoint selectedIndex = FIntPoint(-1,-1);
	TObjectPtr<AMyUnit> SelectedUnit;
protected:
	virtual void ExecuteAction(const FIntPoint& index) override;
	// FIntPoint selected;
public:
	virtual void BeginDestroy() override;
	virtual void Destroyed() override;
};
