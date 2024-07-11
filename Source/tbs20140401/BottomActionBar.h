﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BottomActionBar.generated.h"

class UImage;
struct FTileData;
class UTextBlock;
class UUGameUI_UnitBriefInfo;
class UCmdWidget;
class AMyUnit;
class UUnitPortrait;
class UFirstRolePortrait;
class UUnitInfoDetail;
class UCanvasPanelSlot;
class APlayerController;
/**
 * 
 */
UCLASS(Blueprintable)
class TBS20140401_API UBottomActionBar : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCmdWidget> CmdList;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UFirstRolePortrait> FirstRolePortrait;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUnitPortrait> P1;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUnitPortrait> P2;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUnitPortrait> P3;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUnitPortrait> P4;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUnitPortrait> P5;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUnitPortrait> P6;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUnitPortrait> P7;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUnitPortrait> P8;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUnitPortrait> P9;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUGameUI_UnitBriefInfo> UnitBriefInfo;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUnitInfoDetail> UnitDetailInfoPanel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ProcessTxt;
	
	TArray<TObjectPtr<UUnitPortrait>> Portraits;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> GridType;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> GridStatus;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> GridHeight;

	UPROPERTY(Transient,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> StartBattleHideUI;

	UPROPERTY(Transient,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FinishBattleShowUI;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> FocusImage;
	
	FDelegateHandle EventHandlerGrid;
	FDelegateHandle EventHandlerProcess;
	FDelegateHandle EventHandleUnitSelected;

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerControllerPtr;
	// UPROPERTY()
	// TObjectPtr<UCanvasPanelSlot> CanvasPanelSlot;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;
	
	void OnActionBarChanged(const TArray<TObjectPtr<AMyUnit>>& array);
	void OnEventGrid(const FTileData* TileDataPtr);
	void OnEventProcess(FText ProcessText);
	void OnEventUnitSelect(uint32 UniqueID);
public:
	// TObjectPtr<UCmdWidget> GetCmdPanel()const{return CmdList;}
	TObjectPtr<UCmdWidget> ShowCmdPanel(TObjectPtr<AMyUnit> UnitInstance,int CmdIndex,bool bShowIdle=true)const;
	TObjectPtr<UUGameUI_UnitBriefInfo> GetUnitBriefInfo()const{return UnitBriefInfo;}
	TObjectPtr<UUnitInfoDetail> GetUnitDetailInfo()const{return UnitDetailInfoPanel;}

	void RegisterEvent();
	void ShowFocusUnit(FVector Location);
	void HideFocus();
	
	void PlayHideBattleUI();
	void PlayShowBattleUI();
};
