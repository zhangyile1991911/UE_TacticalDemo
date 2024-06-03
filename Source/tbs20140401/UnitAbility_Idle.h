// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitAbility.h"
#include "UnitAbility_Idle.generated.h"

UCLASS()
class TBS20140401_API UUnitAbility_Idle : public UUnitAbility
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UUnitAbility_Idle();

protected:
	// Called when the game starts or when spawned
	// virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
public:
	// Called every frame
	// virtual void Tick(float DeltaTime) override;
	virtual bool CanExecute()override{return true;}
	virtual bool IsIdle() override{return true;}
	virtual FBattleReport DoCalculation(TObjectPtr<AMyUnit> Target,AGrid* MyGrid)override; 
};
