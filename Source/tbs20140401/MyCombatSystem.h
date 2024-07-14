// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyCombatSystem.generated.h"


enum class ETBSUnitType : uint8;
enum class EUnitDirectType : uint8;

class AAssaultTips;
class AMy_Pawn;
class AMyUnit;
class AGrid;
DECLARE_EVENT_OneParam(AMyCombatSystem,ReSortEvent,const TArray<TObjectPtr<AMyUnit>>&);

USTRUCT()
struct FThreatenInfo
{
	GENERATED_BODY()

	FVector Attacker;
	FVector Defender;	
};


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
	
	UPROPERTY()
	TMap<uint32,TObjectPtr<AMyUnit>> UnitsInCombat;
	UPROPERTY()
	TArray<TObjectPtr<AMyUnit>> UnitsActionPriority;
	UPROPERTY()
	TArray<TObjectPtr<AAssaultTips>> TipsObjectPool;
	UPROPERTY()
	UClass* AssaultTipsClass;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CreateOneTips();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void AddUnitInCombat(const FIntPoint& Index,TObjectPtr<AMy_Pawn> myPawn);
	TObjectPtr<AMyUnit> AddUnitInCombatByType(const FIntPoint& Index,ETBSUnitType UT,EUnitDirectType Direction,AMy_Pawn* MyPawn);
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

	void ShowUnitThreaten(TArray<FThreatenInfo> Infos);
	void HideUnitThreaten();
	TArray<TObjectPtr<AMyUnit>> GetOneSideTeam(int UnitSide);
	void ClearAllUnit();

	TObjectPtr<AMyUnit> GetUnitByType(ETBSUnitType EtbsUnit);
};
