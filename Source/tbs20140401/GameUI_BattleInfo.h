// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameUI_BattleInfo.generated.h"

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

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CooperationTips;

	FVector2D StartFlowPosition;
	FVector2D FinishFlowPosition;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
public:
	UFUNCTION(BlueprintCallable)
	void StartHitNumFlowAnim(AMyUnit* Unit,int Num);
	UFUNCTION(BlueprintCallable)
	void UpdateHitNumFlowAnim(float Value);
	UFUNCTION(BlueprintCallable)
	void FinishHitNumFlowAnim();
	
};

