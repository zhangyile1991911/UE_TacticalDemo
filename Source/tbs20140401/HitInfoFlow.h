// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HitInfoFlow.generated.h"

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
	TObjectPtr<UTextBlock> HitNum;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> IsCritical;

	UPROPERTY(meta=(BindWidget))
    TObjectPtr<UTextBlock> IsBackAtk;
public:
	void StartHitNum(FVector2D Start,FVector2D Finish,float Scale,int Num,bool IsHit);
	void UpdateHitNumFlowAnim(float Value);
	void FinishHitNumFlowAnim();
};
