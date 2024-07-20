// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnitInfoDetailPortrait.generated.h"
enum class ETBSUnitType:uint8;
class UImage;
class UBorder;
class UMyGameInstance;
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

	UPROPERTY()
	UMyGameInstance* GameInstance;
public:
	void ShowUnitPortrait(ETBSUnitType UnitType);
	void HideUnitPortrait();

	void DoHighlight();

	void DoUnhighlight();
};
