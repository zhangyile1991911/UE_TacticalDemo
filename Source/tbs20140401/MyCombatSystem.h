// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyCombatSystem.generated.h"


class AMy_Pawn;
class AMyUnit;
class AGrid;
DECLARE_EVENT_OneParam(AMyCombatSystem,ReSortEvent,const TArray<TObjectPtr<AMyUnit>>&);

UCLASS(Blueprintable)
class TBS20140401_API AMyCombatSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyCombatSystem();
protected:
	
	UPROPERTY()
	TObjectPtr<AGrid> MyGrid;

	UPROPERTY()
	TObjectPtr<AMyUnit> FirstUnit;
	
	TMap<uint32,TObjectPtr<AMyUnit>> UnitsInCombat;

	TArray<TObjectPtr<AMyUnit>> UnitsActionPriority;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void AddUnitInCombat(const FIntPoint& Index,TObjectPtr<AMy_Pawn> myPawn);
	void RemoveUnitInCombat(const FIntPoint& Index);
	void RemoveUnitInCombat(TObjectPtr<AMyUnit> Unit);
	void SetUnitIndexOnGrid(const FIntPoint& Index,TObjectPtr<AMyUnit> Unit);
	void NextAllUnit();
	void ResetAllUnit();
	TArray<TObjectPtr<AMyUnit>> GetAllUnits()
	{
		TArray<TObjectPtr<AMyUnit>> Result;
		UnitsInCombat.GenerateValueArray(Result);
		return MoveTemp(Result);
	}
	TObjectPtr<AMyUnit> SortActionPriority();
	TObjectPtr<AMyUnit> GetFirstUnit()const{return FirstUnit;}
	TArray<TObjectPtr<AMyUnit>> GetThreatenEnemies(TObjectPtr<AMyUnit>);
	ReSortEvent ReSortEvent;
};
