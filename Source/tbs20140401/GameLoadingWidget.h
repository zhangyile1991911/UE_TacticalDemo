﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameLoadingWidget.generated.h"

/**
 * 
 */
UCLASS()
class TBS20140401_API UGameLoadingWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(Transient,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Loading;

	UPROPERTY(Transient,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Blink;
public:
	void PlayLoading();
	void StopLoading();
	void PlayBlink();
	void StopBlink();
};
