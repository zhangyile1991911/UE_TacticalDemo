// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillData.h"
#include "TileData.h"
#include "UnitAbilityAnim.generated.h"

class AMyGridPathfinding;
class AUnitAbilityAnim;
class AMy_Pawn;
class AMyUnit;
class AGrid;
struct FTileData;

DECLARE_EVENT_OneParam(AUnitAbilityAnim,EAbilityComplete,TObjectPtr<AUnitAbilityAnim>)
DECLARE_DELEGATE_OneParam(FAbilityComplete,TObjectPtr<AUnitAbilityAnim>)
UCLASS(Abstract,Blueprintable)
class TBS20140401_API AUnitAbilityAnim : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUnitAbilityAnim();

protected:
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<AMyUnit> OwnerInstance;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FSkillData SkillData;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnAbilityCompleted();
public:

	const FSkillData& GetSkillData()
	{
		return SkillData;
	}
	void SetSkillData(FSkillData Data,TObjectPtr<AMyUnit> OwnerInst)
	{
		SkillData = Data;
		OwnerInstance = OwnerInst;
	}
	const FText& GetAbilityName()const{return SkillData.SkillName;}
	int GetCost()const{return SkillData.SpendPoint;}

	virtual bool CanExecute(){return false;}
	virtual bool IsValidTarget(const FTileData* TileData,AGrid* MyGrid){return false;}
	virtual bool IsValidUnit(TObjectPtr<AMyUnit> Unit);

	virtual FBattleReport DoCalculation(const TArray<TObjectPtr<AMyUnit>>& Targets,AGrid* MyGrid,bool NeedCooperator);
	virtual FBattleReport DoCalculation(TObjectPtr<AMyUnit> Target,AGrid* MyGrid,bool NeedCooperator);

	virtual TArray<FIntPoint> Range(const FIntPoint&){ return TArray<FIntPoint>();}
	virtual TArray<FIntPoint> Indicator(const FIntPoint& Index)
	{
		TArray<FIntPoint> Result;
		Result.Add(Index);
		return Result;
	};
	virtual TArray<TObjectPtr<AMyUnit>> TakeTargets(const FIntPoint& Point,AGrid* MyGrid);

	virtual bool IsShowOnCmd(){return true;}
	
	virtual bool IsIdle(){return false;}
	virtual bool IsCooperate(){return false;}
	virtual bool IsArea(){return false;}
	virtual bool IsHeal(){return false;}

	virtual bool CheckDeviation(int HeightA,int HeightB);
	virtual bool CheckTileDataHeight(const FTileData* TileData, AGrid* MyGrid);
	EAbilityComplete CompletedEvent;
	FAbilityComplete CompletedCallback;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//blueprintで実現する 
	UFUNCTION(BlueprintImplementableEvent)
	void DoAnimation(const FBattleReport& Report,AMy_Pawn* MyPawn);
	
};
