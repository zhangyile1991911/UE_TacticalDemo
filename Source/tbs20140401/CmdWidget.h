// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CmdWidget.generated.h"

class AMyUnit;
class UCmdCell;
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

	TArray<TObjectPtr<UCmdCell>> AbilityList;

	int SelectedIndex = -1;
protected:
	virtual void NativeConstruct() override;

public:
	void RefreshUnitCmd(TObjectPtr<AMyUnit> Unit);
	void SelectCmd(int index);
protected:
	virtual void NativeDestruct() override;
};
