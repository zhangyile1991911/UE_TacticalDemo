// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyAction.h"
#include "Action_SetTileType.generated.h"

UCLASS()
class TBS20140401_API AAction_SetTileType : public AMyAction
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAction_SetTileType();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void ExecuteAction(const FIntPoint& index) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
