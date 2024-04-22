// Fill out your copyright notice in the Description page of Project Settings.


#include "MySpinBox_WithName_Vector2.h"
#include "Components/SpinBox.h"
#include "Components/TextBlock.h"

void UMySpinBox_WithName_Vector2::NativePreConstruct()
{
	Super::NativePreConstruct();

}

void UMySpinBox_WithName_Vector2::NativeConstruct()
{
	Super::NativeConstruct();
	Text_Name->SetText(FText::FromString(Name));
	
	SpinBox_Value_X->SetMinSliderValue(MinMax_Slider.X);
	SpinBox_Value_X->SetMaxSliderValue(MinMax_Slider.Y);
	SpinBox_Value_Y->SetMinSliderValue(MinMax_Slider.X);
	SpinBox_Value_Y->SetMaxSliderValue(MinMax_Slider.Y);
	
	SpinBox_Value_X->SetMinValue(MinMax_Value.X);
	SpinBox_Value_X->SetMaxValue(MinMax_Value.Y);
	SpinBox_Value_Y->SetMinValue(MinMax_Value.X);
	SpinBox_Value_Y->SetMaxValue(MinMax_Value.Y);
	
	SpinBox_Value_X->SetValue(Value.X);
	SpinBox_Value_Y->SetValue(Value.Y);
	
	SpinBox_Value_X->SetMinFractionalDigits(MinMax_FractDigits.X);
	SpinBox_Value_X->SetMinFractionalDigits(MinMax_FractDigits.Y);
	SpinBox_Value_Y->SetMinFractionalDigits(MinMax_FractDigits.X);
	SpinBox_Value_Y->SetMinFractionalDigits(MinMax_FractDigits.Y);
	
	SpinBox_Value_X->SetAlwaysUsesDeltaSnap(AlwaysSnapToDelta);
	SpinBox_Value_Y->SetAlwaysUsesDeltaSnap(AlwaysSnapToDelta);
	
	SpinBox_Value_X->SetDelta(Delta);
	SpinBox_Value_Y->SetDelta(Delta);
	
	SpinBox_Value_X->OnValueChanged.AddDynamic(this,&UMySpinBox_WithName_Vector2::ListenOnXValueChange);
	SpinBox_Value_Y->OnValueChanged.AddDynamic(this,&UMySpinBox_WithName_Vector2::ListenOnYValueChange);
}

void UMySpinBox_WithName_Vector2::SetValue(FVector2D val)
{
	SpinBox_Value_X->SetValue(val.X);
	SpinBox_Value_Y->SetValue(val.Y);
}

void UMySpinBox_WithName_Vector2::ListenOnXValueChange(float newVal)
{
	Value.X = newVal;
	Vector2Topic.Broadcast(Value);
}

void UMySpinBox_WithName_Vector2::ListenOnYValueChange(float newVal)
{
	Value.Y = newVal;
	Vector2Topic.Broadcast(Value);
}