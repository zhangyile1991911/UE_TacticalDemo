// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyButtonList_Units.h"
#include "Blueprint/UserWidget.h"
#include "UnitInfoDetailPortrait.generated.h"

class UImage;
class UBorder;
/**
 * 
 */
UCLASS()
class TBS20140401_API UUnitInfoDetailPortrait : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBorder> PortraitBorder;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> PortraitImage;

	UPROPERTY(Transient,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Highlight;
	
	UPROPERTY(Transient,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> UnHighlight;
public:
	void ShowUnitPortrait(EUnitType UnitType);
	void HideUnitPortrait();

	void DoHighlight();

	void DoUnhighlight();
};
