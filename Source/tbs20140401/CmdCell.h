// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SkillData.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CmdCell.generated.h"

struct FSkillData;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class TBS20140401_API UCmdCell : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> AbilityIcon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> AbilityName;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> AP1;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> AP2;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> AP3;

	UPROPERTY(Transient,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Selected;
	
	UPROPERTY(Transient,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> UnSelected;
	
	TArray<TObjectPtr<UImage>> APList;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void RefreshCell(const FText& Name,int Cost);
	void DoSelected();
	void DoUnSelected();
	void HideContent();
};
