// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDebugMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

void UMyDebugMenu::tab1()
{
	if(TabsContent->GetActiveWidgetIndex() == 1)
	{
		TabsContent->SetActiveWidgetIndex(0);
	}
	else
	{
		TabsContent->SetActiveWidgetIndex(1);
	}
	
	for(int i = 0;i < TabButtons.Num();i++)
	{
		if(TabButtons[i] == nullptr)continue;
		if(i == TabsContent->GetActiveWidgetIndex())
			TabButtons[i]->SetBackgroundColor(FLinearColor::Blue);
		else
			TabButtons[i]->SetBackgroundColor(DefaultColor);
	}
}

void UMyDebugMenu::tab2()
{
	if(TabsContent->GetActiveWidgetIndex() == 2)
	{
		TabsContent->SetActiveWidgetIndex(0);
	}
	else
	{
		TabsContent->SetActiveWidgetIndex(2);	
	}
	for(int i = 0;i < TabButtons.Num();i++)
	{
		if(TabButtons[i] == nullptr)continue;
		if(i == TabsContent->GetActiveWidgetIndex())
			TabButtons[i]->SetBackgroundColor(FLinearColor::Blue);
		else
			TabButtons[i]->SetBackgroundColor(DefaultColor);
	}
}

void UMyDebugMenu::tab3()
{
	if(TabsContent->GetActiveWidgetIndex() == 3)
	{
		TabsContent->SetActiveWidgetIndex(0);
	}
	else
	{
		TabsContent->SetActiveWidgetIndex(3);	
	}
	for(int i = 0;i < TabButtons.Num();i++)
	{
		if(TabButtons[i] == nullptr)continue;
		if(i == TabsContent->GetActiveWidgetIndex())
			TabButtons[i]->SetBackgroundColor(FLinearColor::Blue);
		else
			TabButtons[i]->SetBackgroundColor(DefaultColor);
	}
}

void UMyDebugMenu::tab4()
{
	if(TabsContent->GetActiveWidgetIndex() == 4)
	{
		TabsContent->SetActiveWidgetIndex(0);
	}
	else
	{
		TabsContent->SetActiveWidgetIndex(4);	
	}
	for(int i = 0;i < TabButtons.Num();i++)
	{
		if(TabButtons[i] == nullptr)continue;
		if(i == TabsContent->GetActiveWidgetIndex())
			TabButtons[i]->SetBackgroundColor(FLinearColor::Blue);
		else
			TabButtons[i]->SetBackgroundColor(DefaultColor);
	}
}

void UMyDebugMenu::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Tab1->OnClicked.AddDynamic(this,&UMyDebugMenu::tab1);
	Button_Tab2->OnClicked.AddDynamic(this,&UMyDebugMenu::tab2);
	Button_Tab3->OnClicked.AddDynamic(this,&UMyDebugMenu::tab3);
	Button_Tab4->OnClicked.AddDynamic(this,&UMyDebugMenu::tab4);

	TabButtons.Add(nullptr);
	TabButtons.Add(Button_Tab1);
	TabButtons.Add(Button_Tab2);
	TabButtons.Add(Button_Tab3);
	TabButtons.Add(Button_Tab4);
}

void UMyDebugMenu::NativeDestruct()
{
	Button_Tab1->OnClicked.Clear();
	Button_Tab2->OnClicked.Clear();
	Button_Tab3->OnClicked.Clear();
	Button_Tab4->OnClicked.Clear();
	TabButtons.Empty();
	Super::NativeDestruct();
}
