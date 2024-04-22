// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MySpinBox_WithName.generated.h"

class UTextBlock;
class USpinBox;
/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(OnFloatChanged,float);
UCLASS(Abstract)
class TBS20140401_API UMySpinBox_WithName : public UUserWidget
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<USpinBox> SpinBox_Value;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Name;

protected:
	virtual void NativePreConstruct() override;
	void SetValue(float);
	
	UFUNCTION()
	void ListenOnValueChange(float newVal);

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString Name;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool AlwaysSnapToDelta;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Value;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Delta;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector2f MinMax_FractDigits;;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector2f MinMax_Value;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector2f MinMax_Slider;
public:
	
	OnFloatChanged TopicChanged;
};
