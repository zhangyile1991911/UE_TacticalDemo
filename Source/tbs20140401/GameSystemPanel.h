// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameSystemPanel.generated.h"

class UGameLoadingWidget;
/**
 * 
 */
UCLASS()
class TBS20140401_API UGameSystemPanel : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UGameLoadingWidget> GameLoading;
public:
	void ShowLoading();
	void HideLoading();
	void WaitingEnter();
};
