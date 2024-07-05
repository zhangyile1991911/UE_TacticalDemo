// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CmdWidget.generated.h"

class UTextBlock;
class AMyUnit;
class UCmdCell;
class AUnitAbilityAnim;
/**
 * 
 */
UCLASS()
class TBS20140401_API UCmdWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCmdCell> AbilityCellA;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCmdCell> AbilityCellB;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCmdCell> AbilityCellC;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCmdCell> AbilityCellD;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCmdCell> AbilityCellE;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCmdCell> AbilityCellF;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCmdCell> AbilityCellG;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> DamageTitle;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> DamageNum;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> RangeTitle;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> RangeNum;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Describe;

	TArray<TObjectPtr<UCmdCell>> AbilityList;

	TArray<TObjectPtr<AUnitAbilityAnim>> ArrayOfAbility;
	int SelectedIndex = -1;
	int AbilityNum = 0;
protected:
	virtual void NativeConstruct() override;

public:
	void RefreshUnitCmd(TObjectPtr<AMyUnit> Unit,bool bShowIdle);
	void SelectCmd(int index);
protected:
	virtual void NativeDestruct() override;
};
