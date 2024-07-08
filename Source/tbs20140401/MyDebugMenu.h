// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyDebugMenu.generated.h"

class UMyTab_Game;
class UMyTab_PathFinding;
class UMyTab_Grid;
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

	UFUNCTION()
	void tab4();

	UFUNCTION()
	void tab5();
protected:
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UButton> Button_Tab1;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UButton> Button_Tab2;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UButton> Button_Tab3;
	
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UButton> Button_Tab4;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UButton> Button_Tab5;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> TabsContent;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UMyTab_Grid> MyTab_Grid;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UMyTab_PathFinding> MyTab_PathFinding;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UMyTab_Game> MyTab_Game;
	

	TArray<TObjectPtr<UButton>> TabButtons;
	FLinearColor DefaultColor = FLinearColor(0.015686f,0.015686f,0.015686f,1.0f);
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
