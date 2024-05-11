// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitData.h"
#include "Blueprint/UserWidget.h"
#include "MyButtonList_Units.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FUnitTypeChange,EUnitType,UnitType);

enum class EUnitType : uint8;
class UButtonUnit;
class UHorizontalBox;
/**
 * 
 */
UCLASS(Abstract)
class TBS20140401_API UMyButtonList_Units : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="FromCPP",meta=(BindWidget))
	TObjectPtr<UHorizontalBox>  hBox_Units;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// TSubclassOf<UUserWidget> MyButtonBlueprint;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="FromCPP")
	EUnitType CurrentSelectedType = EUnitType::Warrior;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="FromCPP")
	bool IsLookingRight = false;

	UPROPERTY(Blueprintable,BlueprintReadWrite,Category="FromCPP")
	TArray<TObjectPtr<UButtonUnit>> ButtonUnits;

	UPROPERTY(Blueprintable,BlueprintReadWrite,Category="FromCPP")
	TObjectPtr<UButtonUnit> CurrentSelectedButton;
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnClickUnit(UButtonUnit* param);

	TObjectPtr<UButtonUnit> GetCurrentSelectedButton()const{return CurrentSelectedButton;}
public:
	EUnitType GetCurrentSelectedType()const{return CurrentSelectedType;}
	FUnitTypeChange OnUnitTypeChanged;
};
