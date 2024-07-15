// Fill out your copyright notice in the Description page of Project Settings.


#include "MyButtonAction.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UMyButtonAction::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UE_LOG(LogTemp,Log,TEXT("UMyButtonAction::NativeOnInitialized()"))
	myText->SetText(ButtonText);
}

void UMyButtonAction::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp,Log,TEXT("UMyButtonAction::NativeConstruct()"))
	myButton->OnClicked.AddDynamic(this,&UMyButtonAction::OnButtonClick);

}

void UMyButtonAction::NativeDestruct()
{
	Super::NativeDestruct();
	UE_LOG(LogTemp,Log,TEXT("UMyButtonAction::NativeDestruct()"))
}

void UMyButtonAction::OnButtonClick()
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
	ButtonActionCB.Broadcast();
}


void UMyButtonAction::SetUnSelected()
{
	selected = false;
	
	myButton->SetBackgroundColor(FLinearColor::Gray);
	
}
#if WITH_EDITOR
void UMyButtonAction::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UE_LOG(LogTemp,Log,TEXT("UMyButtonAction::PostEditChangeProperty()"))
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if(PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UMyButtonAction,ButtonText))
	{
		if(myText)
		{
			myText->SetText(ButtonText);
		}
	}
}
#endif


