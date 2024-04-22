// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyButtonAction.generated.h"

class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class TBS20140401_API UMyButtonAction : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> myButton;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> myText;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText ButtonText;
	
	UPROPERTY(EditAnywhere,Blueprintable,meta=(AllowedClasses="AMyAction"))
	UClass* LeftAction;

	UPROPERTY(EditAnywhere,Blueprintable,meta=(AllowedClasses="AMyAction"))
	UClass* RightAction;

	bool selected = false;
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnButtonClick();
public:
#ifdef UE_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
};
