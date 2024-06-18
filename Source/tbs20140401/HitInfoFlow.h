// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HitInfoFlow.generated.h"

class UHorizontalBox;
class UTextBlock;
class UCanvasPanelSlot;
/**
 * 
 */
UCLASS()
class TBS20140401_API UHitInfoFlow : public UUserWidget
{
	GENERATED_BODY()

	FVector2D StartPos;
	FVector2D FinishPos;
	TObjectPtr<UCanvasPanelSlot> MySlotPtr;
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHorizontalBox> TextBlockBox;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> HitNumTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> IsCriticalTextBlock;

	UPROPERTY(meta=(BindWidget))
    TObjectPtr<UTextBlock> IsBackAtkTextBlock;
public:
	void StartHitNum(FVector2D Start,FVector2D Finish,float Scale,int Num,bool bIsHit,bool bIsCritical,bool bIsBackAtk);
	void UpdateHitNumFlowAnim(float Value);
	void FinishHitNumFlowAnim();
};
