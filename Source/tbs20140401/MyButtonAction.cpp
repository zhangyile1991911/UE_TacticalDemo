// Fill out your copyright notice in the Description page of Project Settings.


#include "MyButtonAction.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UMyButtonAction::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	myText->SetText(ButtonText);
}

void UMyButtonAction::NativeConstruct()
{
	Super::NativeConstruct();

	myButton->OnClicked.AddDynamic(this,&UMyButtonAction::OnButtonClick);

}

void UMyButtonAction::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMyButtonAction::OnButtonClick()
{
	selected = !selected;
	if(selected)
	{
		myButton->SetBackgroundColor(FLinearColor::Black);
	}
	else
	{
		myButton->SetBackgroundColor(FLinearColor::Blue);
	}
	
}

#ifdef UE_EDITOR
void UMyButtonAction::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
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


