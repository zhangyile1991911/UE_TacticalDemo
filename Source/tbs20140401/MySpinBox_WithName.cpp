// Fill out your copyright notice in the Description page of Project Settings.


#include "MySpinBox_WithName.h"

#include "Components/SpinBox.h"
#include "Components/TextBlock.h"

void UMySpinBox_WithName::NativePreConstruct()
{
	Super::NativePreConstruct();

	Text_Name->SetText(FText::FromString(Name));
	
	SpinBox_Value->SetMinSliderValue(MinMax_Slider.X);
	SpinBox_Value->SetMaxSliderValue(MinMax_Slider.Y);
	
	SpinBox_Value->SetMinValue(MinMax_Value.X);
	SpinBox_Value->SetMinValue(MinMax_Value.Y);
	
	SpinBox_Value->SetValue(Value);
	
	SpinBox_Value->SetMinFractionalDigits(MinMax_FractDigits.X);
	SpinBox_Value->SetMinFractionalDigits(MinMax_FractDigits.Y);
	
	SpinBox_Value->SetAlwaysUsesDeltaSnap(AlwaysSnapToDelta);
	
	SpinBox_Value->SetDelta(Delta);
	
}

void UMySpinBox_WithName::NativeConstruct()
{
	SpinBox_Value->OnValueChanged.AddDynamic(this,&UMySpinBox_WithName::ListenOnValueChange);
}

void UMySpinBox_WithName::SetValue(float val)
{
	SpinBox_Value->SetValue(val);
}

void UMySpinBox_WithName::ListenOnValueChange(float newVal)
{
	// Value = newVal;
	TopicChanged.Broadcast(newVal);
}