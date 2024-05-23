// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnitPortrait.generated.h"

class AMyUnit;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class TBS20140401_API UUnitPortrait : public UUserWidget
{
	GENERATED_BODY()
protected:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Portrait;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> AP1;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> AP2;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> AP3;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SeqNum;

	TArray<TObjectPtr<UImage>> APs;
public:
	void RefreshUnitIcon(TObjectPtr<AMyUnit> Unit);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
