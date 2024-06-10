// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyButtonActionWithUnit.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TBS20140401_API UMyButtonActionWithUnit : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> myButton;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> myText;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText ButtonText;
	
	
	UPROPERTY(EditAnywhere,Blueprintable,meta=(AllowedClasses="MyAction"))
	UClass* LeftAction;

	UPROPERTY(EditAnywhere,Blueprintable,meta=(AllowedClasses="MyAction"))
	UClass* RightAction;
	
	bool selected = false;
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnButtonClick();
public:
	UClass* GetLeftAction(){return LeftAction;}
	UClass* GetRightAction(){return RightAction;}
	
	// ButtonActionCB ButtonActionCB;
	bool IsSelected(){return selected;}
	void SetUnSelected();
	
};