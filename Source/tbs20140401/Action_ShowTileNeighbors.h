// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyAction.h"
#include "Action_ShowTileNeighbors.generated.h"

class AMyGridPathfinding;
/**
 * 
 */
UCLASS()
class TBS20140401_API AAction_ShowTileNeighbors : public AMyAction
{
	GENERATED_BODY()
protected:
	FIntPoint selectedIndex;
	TArray<FIntPoint> CurNeighbors;
	UPROPERTY()
	AMyGridPathfinding* MyPathFinding;
protected:
	virtual void BeginPlay() override;
	virtual void ExecuteAction(const FIntPoint& index) override;
	virtual void BeginDestroy() override;
	virtual void Destroyed() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;	
};
