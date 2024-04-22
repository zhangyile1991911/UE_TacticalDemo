// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMySpinBox_WithName_Vector3.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(OnVector3Changed,const FVector&);
class UTextBlock;
class USpinBox;
/**
 * 
 */

UCLASS()
class TBS20140401_API UUMySpinBox_WithName_Vector3 : public UUserWidget
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<USpinBox> SpinBox_Value_X;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<USpinBox> SpinBox_Value_Y;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<USpinBox> SpinBox_Value_Z;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Name;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	
	
	UFUNCTION()
	void ListenOnXValueChange(float newVal);

	UFUNCTION()
	void ListenOnYValueChange(float newVal);

	UFUNCTION()
	void ListenOnZValueChange(float newVal);

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString Name=FString(TEXT(""));
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool AlwaysSnapToDelta;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector Value;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Delta;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector2f MinMax_FractDigits;;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector2f MinMax_Value;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector2f MinMax_Slider;
public:
	OnVector3Changed	Vector3Topic;
	void SetValue(FVector);
};
