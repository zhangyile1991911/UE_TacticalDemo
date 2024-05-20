// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyButtonList_Units.h"
#include "GameFramework/Actor.h"
#include "MyCombatSystem.generated.h"

class AMy_Pawn;
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

	TArray<TObjectPtr<AMyUnit>> UnitsActionPriority;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SortActionPriority();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void AddUnitInCombat(const FIntPoint& Index,TObjectPtr<AMy_Pawn> myPawn);
	void RemoveUnitInCombat(const FIntPoint& Index);
	void SetUnitIndexOnGrid(const FIntPoint& Index,TObjectPtr<AMyUnit> Unit);
};
