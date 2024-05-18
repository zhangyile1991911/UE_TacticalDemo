// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyAction.h"
#include "Action_FindPath.generated.h"

class AMyGridPathfinding;
/**
 * 
 */
UCLASS()
class TBS20140401_API AAction_FindPath : public AMyAction
{
	GENERATED_BODY()
protected:
	// UPROPERTY()
	// AMyGridPathfinding* MyGridPathfinding;

	// TArray<FIntPoint> Path;
	FIntPoint Start;
	FIntPoint Finish;
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void WaitPathFinding(TArray<FIntPoint>);
public:
	virtual void ExecuteAction(const FIntPoint& index) override;
};
