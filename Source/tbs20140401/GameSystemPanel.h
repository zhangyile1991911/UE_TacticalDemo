// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameSystemPanel.generated.h"

class UGameOverWidget;
class UGameLoadingWidget;
/**
 * 
 */
UCLASS(Blueprintable)
class TBS20140401_API UGameSystemPanel : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UGameLoadingWidget> GameLoading;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UGameOverWidget> GameOver;
public:
	void ShowLoading(int LevelNum);
	void HideLoading();
	void WaitingEnter();
	void ShowGameOver();
};
