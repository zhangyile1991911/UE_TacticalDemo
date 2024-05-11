// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyButtonList_Units.h"
#include "GameFramework/Actor.h"
#include "MyCombatSystem.generated.h"

class AMyUnit;
class AGrid;

UCLASS()
class TBS20140401_API AMyCombatSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyCombatSystem();
protected:
	
	UPROPERTY()
	TObjectPtr<AGrid> MyGrid;

	TMap<FIntPoint,TObjectPtr<AMyUnit>> UnitsInCombat;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void AddUnitInCombat(const FIntPoint& Index,EUnitType UnitType);
	void RemoveUnitInCombat(const FIntPoint& Index);
	void SetUnitIndexOnGrid(const FIntPoint& Index,TObjectPtr<AMyUnit> Unit);
};
