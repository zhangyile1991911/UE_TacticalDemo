// Fill out your copyright notice in the Description page of Project Settings.


#include "UMySpinBox_WithName_Vector3.h"
#include "Components/SpinBox.h"
#include "Components/TextBlock.h"


void UUMySpinBox_WithName_Vector3::NativePreConstruct()
{
	Super::NativePreConstruct();

}

void UUMySpinBox_WithName_Vector3::NativeConstruct()
{
	Super::NativeConstruct();

	Text_Name->SetText(FText::FromString(Name));
	
	SpinBox_Value_X->SetMinSliderValue(MinMax_Slider.X);
	SpinBox_Value_X->SetMaxSliderValue(MinMax_Slider.Y);
	SpinBox_Value_Y->SetMinSliderValue(MinMax_Slider.X);
	SpinBox_Value_Y->SetMaxSliderValue(MinMax_Slider.Y);
	SpinBox_Value_Z->SetMinSliderValue(MinMax_Slider.X);
	SpinBox_Value_Z->SetMaxSliderValue(MinMax_Slider.Y);
	
	SpinBox_Value_X->SetMinValue(MinMax_Value.X);
	SpinBox_Value_X->SetMaxValue(MinMax_Value.Y);
	SpinBox_Value_Y->SetMinValue(MinMax_Value.X);
	SpinBox_Value_Y->SetMaxValue(MinMax_Value.Y);
	SpinBox_Value_Z->SetMinValue(MinMax_Value.X);
	SpinBox_Value_Z->SetMaxValue(MinMax_Value.Y);
	
	SpinBox_Value_X->SetValue(Value.X);
	SpinBox_Value_Y->SetValue(Value.Y);
	SpinBox_Value_Y->SetValue(Value.Z);
	
	SpinBox_Value_X->SetMinFractionalDigits(MinMax_FractDigits.X);
	SpinBox_Value_X->SetMinFractionalDigits(MinMax_FractDigits.Y);
	SpinBox_Value_Y->SetMinFractionalDigits(MinMax_FractDigits.X);
	SpinBox_Value_Y->SetMinFractionalDigits(MinMax_FractDigits.Y);
	SpinBox_Value_Z->SetMinFractionalDigits(MinMax_FractDigits.X);
	SpinBox_Value_Z->SetMinFractionalDigits(MinMax_FractDigits.Y);
	
	SpinBox_Value_X->SetAlwaysUsesDeltaSnap(AlwaysSnapToDelta);
	SpinBox_Value_Y->SetAlwaysUsesDeltaSnap(AlwaysSnapToDelta);
	SpinBox_Value_Z->SetAlwaysUsesDeltaSnap(AlwaysSnapToDelta);
	
	SpinBox_Value_X->SetDelta(Delta);
	SpinBox_Value_Y->SetDelta(Delta);
	SpinBox_Value_Z->SetDelta(Delta);
	
	SpinBox_Value_X->OnValueChanged.AddDynamic(this,&UUMySpinBox_WithName_Vector3::ListenOnXValueChange);
	SpinBox_Value_Y->OnValueChanged.AddDynamic(this,&UUMySpinBox_WithName_Vector3::ListenOnYValueChange);
	SpinBox_Value_Z->OnValueChanged.AddDynamic(this,&UUMySpinBox_WithName_Vector3::ListenOnZValueChange);
}

void UUMySpinBox_WithName_Vector3::SetValue(FVector val)
{
	SpinBox_Value_X->SetValue(val.X);
	SpinBox_Value_Y->SetValue(val.Y);
	SpinBox_Value_Z->SetValue(val.Z);
}

void UUMySpinBox_WithName_Vector3::ListenOnXValueChange(float newVal)
{
	Value.X = newVal;
	Vector3Topic.Broadcast(Value);
}

void UUMySpinBox_WithName_Vector3::ListenOnYValueChange(float newVal)
{
	Value.Y = newVal;
	Vector3Topic.Broadcast(Value);
}

void UUMySpinBox_WithName_Vector3::ListenOnZValueChange(float newVal)
{
	Value.Z = newVal;
	Vector3Topic.Broadcast(Value);
}