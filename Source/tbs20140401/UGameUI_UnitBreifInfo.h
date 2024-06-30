// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UGameUI_UnitBreifInfo.generated.h"

class UOverlay;
class AMyUnit;
class UProgressBar;
class UImage;
class UTextBlock;
class UCanvasPanelSlot;
/**
 * 
 */
UCLASS(Blueprintable)
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
	TObjectPtr<UImage> HitIcon;

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

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> InfoNode;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> ConfirmNode;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> TabNode;
	
	TArray<TObjectPtr<UImage>> APList;
	UPROPERTY()
	TObjectPtr<UCanvasPanelSlot> CanvasPanelSlot;

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerControllerPtr;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	void RefreshUnitBriefInfo(TObjectPtr<AMyUnit> Attacker,TObjectPtr<AMyUnit> Defender,float HitPercent);
public:
	void ShowMoveOnly();
	void ShowDetailOnly();
	void ShowSelfCmd(TObjectPtr<AMyUnit> Attacker);
	void ShowConfirmCmd(const FText& ConfirmTxt);
	void ShowTarget(TObjectPtr<AMyUnit> Attacker,TObjectPtr<AMyUnit> Defender,float HitPercent,FText ConfirmTxt,FText DetailTxt);
	void ShowTargetInfoAndTab(TObjectPtr<AMyUnit> Defender,float HitPercent);
	void ShowTargetInfoAndConfirmAndTab(TObjectPtr<AMyUnit> Defender,FText ConfirmTxt,FText TabTxt);
	TObjectPtr<UCanvasPanelSlot> GetCanvasPanelSlot();
	void UpdateWidgetPosition(const FVector& Location);
};

