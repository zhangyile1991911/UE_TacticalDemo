// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MySpinBox_WithName_Vector2.generated.h"
class UTextBlock;
class USpinBox;
DECLARE_MULTICAST_DELEGATE_OneParam(OnVector2Changed,const FVector2D&);
/**
 * 
 */
UCLASS()
class TBS20140401_API UMySpinBox_WithName_Vector2 : public UUserWidget
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<USpinBox> SpinBox_Value_X;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<USpinBox> SpinBox_Value_Y;
	

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Name;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	
	
	UFUNCTION()
	void ListenOnXValueChange(float newVal);

	UFUNCTION()
	void ListenOnYValueChange(float newVal);

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString Name=FString(TEXT(""));
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool AlwaysSnapToDelta;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector2D Value;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Delta;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector2f MinMax_FractDigits;;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector2f MinMax_Value;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector2f MinMax_Slider;
public:
	OnVector2Changed Vector2Topic;
	void SetValue(FVector2D);
};
