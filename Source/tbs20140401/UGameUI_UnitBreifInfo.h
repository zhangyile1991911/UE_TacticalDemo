// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UGameUI_UnitBreifInfo.generated.h"

class AMyUnit;
class UProgressBar;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TBS20140401_API UUGameUI_UnitBriefInfo : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> LevelText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> UnitNameText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> UnitIcon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> AP1;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> AP2;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> AP3;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> HPBar;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> HitPerText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CurHPText;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> MaxHPText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> ConfirmIcon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ConfirmText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> TabIcon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TabText;

	
	TArray<TObjectPtr<UImage>> APList;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void RefreshUnitBriefInfo(TObjectPtr<AMyUnit> Attacker,TObjectPtr<AMyUnit> Defender);
};

