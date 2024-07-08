// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyTab_Game.generated.h"

class AMy_Pawn;
class AMyHUD;
class UButton;
/**
 * 
 */
UCLASS(Abstract)
class TBS20140401_API UMyTab_Game : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UButton> StartGameBtn;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UButton> LoadGridDataBtn;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget))
	TObjectPtr<UButton> SaveGridDataBtn;

	UPROPERTY()
	TObjectPtr<AMyHUD> MyHUDInstance;

	UPROPERTY()
	TObjectPtr<AMy_Pawn> MyPawnInstance;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnStartGameClicked();
	
	UFUNCTION()
	void OnSaveGridDataClicked();

	UFUNCTION()
	void OnLoadGirdDataClicked();
};
