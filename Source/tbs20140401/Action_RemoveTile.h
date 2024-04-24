﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyAction.h"
#include "Action_RemoveTile.generated.h"

UCLASS()
class TBS20140401_API AAction_RemoveTile : public AMyAction
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAction_RemoveTile();
	virtual void ExecuteAction(const FIntPoint& index) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
