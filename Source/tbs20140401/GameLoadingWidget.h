// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameLoadingWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TBS20140401_API UGameLoadingWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UTextBlock> LoadTxt;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UTextBlock> CompleteTxt;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UImage> PA;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UImage> PB;
	
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UImage> PC;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UImage> Tutorial;
	
	UPROPERTY(Transient,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Loading;

	UPROPERTY(Transient,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Blink;
protected:
	virtual void NativeConstruct() override;
public:
	void PlayLoading();
	void StopLoading();
	void PlayBlink();
	void StopBlink();
	void ShowTutorial();
	void HideTutorial();
	
};
