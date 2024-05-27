// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillData.h"
#include "UObject/Object.h"
#include "UnitAbility.generated.h"


class UUnitAbility;
DECLARE_EVENT_OneParam(UUnitAbility,EAbilityComplete,TObjectPtr<UUnitAbility>)
DECLARE_DELEGATE_OneParam(FAbilityComplete,TObjectPtr<UUnitAbility>)

struct FTileData;
class AMyUnit;
/**
 * 
 */
UCLASS(Abstract)
class TBS20140401_API UUnitAbility : public UObject
{
	GENERATED_BODY()
protected:
	FSkillData SkillData;

public:
	virtual void BeginDestroy() override;

protected:
	UPROPERTY()
	TObjectPtr<AMyUnit> OwnerInstance;
public:
	EAbilityComplete CompletedEvent;
	FAbilityComplete CompletedCallback;
	
	const FSkillData& GetSkillData()
	{
		return SkillData;
	}
	void SetSkillData(FSkillData Data,TObjectPtr<AMyUnit> Owner)
	{
		SkillData = Data;
		OwnerInstance = Owner;
	}
	const FText& GetAbilityName()const{return SkillData.SkillName;}
	int GetCost()const{return SkillData.SpendPoint;}
	
	virtual bool CanExecute(){return false;}
	virtual bool IsValidTarget(const FTileData& TileData){return false;}
	virtual void Execute(){}
	virtual TArray<FIntPoint> Range(const FIntPoint&){ return TArray<FIntPoint>();}

	UFUNCTION(BlueprintCallable)
	void OnAbilityCompleted();
};
