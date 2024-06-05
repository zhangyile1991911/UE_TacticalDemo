// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameUI_BattleInfo.generated.h"

class UVerticalBox;
class UCanvasPanelSlot;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TBS20140401_API UGameUI_BattleInfo : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> HitNum;
	UPROPERTY()
	TObjectPtr<UCanvasPanelSlot> HitNumSlot;
	
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> BackAtkTips;
	// UPROPERTY()
	// TObjectPtr<UCanvasPanelSlot> BackAtkTipSlot;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CooperationTips;
	// UPROPERTY()
	// TObjectPtr<UCanvasPanelSlot> CooperationTipsSlot;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> TipsGroup;
	UPROPERTY()
	TObjectPtr<UCanvasPanelSlot> TipsGroupSlot;
	

	FVector2D StartFlowPosition;
	FVector2D FinishFlowPosition;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	bool LocationToScreenPosition(FVector UnitLocation,FVector2D& ScreenLocation,float& Scale);
public:
	UFUNCTION(BlueprintCallable)
	void StartHitNumFlowAnim(AMyUnit* Unit,int Num,bool IsHit);
	UFUNCTION(BlueprintCallable)
	void UpdateHitNumFlowAnim(float Value);
	UFUNCTION(BlueprintCallable)
	void FinishHitNumFlowAnim();

	void ShowBackAtkTips(AMyUnit* Unit);
	void HideBackAtkTips();

	void ShowCooperatorTips(AMyUnit* Unit);
	void HideCooperatorTips();
	
	
};

