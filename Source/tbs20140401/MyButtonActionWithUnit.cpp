// Fill out your copyright notice in the Description page of Project Settings.


#include "MyButtonActionWithUnit.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UMyButtonActionWithUnit::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	myText->SetText(ButtonText);
}

void UMyButtonActionWithUnit::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp,Log,TEXT("UMyButtonAction::NativeConstruct()"))
	myButton->OnClicked.AddDynamic(this,&UMyButtonActionWithUnit::OnButtonClick);

}

void UMyButtonActionWithUnit::NativeDestruct()
{
	Super::NativeDestruct();
	
}

void UMyButtonActionWithUnit::OnButtonClick()
{
	selected = !selected;
	if(selected)
	{
		myButton->SetBackgroundColor(FLinearColor::Blue);
	}
	else
	{
		myButton->SetBackgroundColor(FLinearColor::Gray);
	}
	// ButtonActionCB.Broadcast();
}