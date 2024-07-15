// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TBS20140401_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(Transient,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Blink;
public:
	void ShowGameOver();
};
