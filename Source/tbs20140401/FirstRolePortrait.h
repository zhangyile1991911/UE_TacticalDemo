// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FirstRolePortrait.generated.h"

class AMyUnit;
class UProgressBar;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class TBS20140401_API UFirstRolePortrait : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Portrait;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> JobIcon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Name;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> JobText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> HPText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> HPBar;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> AP1;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> AP2;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> AP3;

	TArray<TObjectPtr<UImage>> APs;
public:
	void RefreshByUnit(TObjectPtr<AMyUnit> Unit);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
