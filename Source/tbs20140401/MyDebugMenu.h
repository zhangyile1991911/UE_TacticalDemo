// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyDebugMenu.generated.h"

class UWidgetSwitcher;
/**
 * 
 */
class UButton;

UCLASS(Abstract)
class TBS20140401_API UMyDebugMenu : public UUserWidget
{
	GENERATED_BODY()
	UFUNCTION()
	void tab1();
	
	UFUNCTION()
	void tab2();
	
	UFUNCTION()
	void tab3();
protected:
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UButton> Button_Tab1;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UButton> Button_Tab2;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UButton> Button_Tab3;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> TabsContent;

	TArray<TObjectPtr<UButton>> TabButtons;
	FLinearColor DefaultColor = FLinearColor(0.015686f,0.015686f,0.015686f,1.0f);
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
