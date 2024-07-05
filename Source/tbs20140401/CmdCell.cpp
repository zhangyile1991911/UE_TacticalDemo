// Fill out your copyright notice in the Description page of Project Settings.


#include "CmdCell.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCmdCell::NativeConstruct()
{
	Super::NativeConstruct();
	
	APList.Add(AP1);
	APList.Add(AP2);
	APList.Add(AP3);
}

void UCmdCell::NativeDestruct()
{
	Super::NativeDestruct();
	for(TObjectPtr<UImage>& img : APList)
	{
		img = nullptr;
	}
	APList.Empty();
	
}

void UCmdCell::RefreshCell(const FText& Name,int Cost)
{
	for(TObjectPtr<UImage> ap : APList)
	{
		ap->SetVisibility(ESlateVisibility::Hidden);
	}
	
	for(int i = 0;i < Cost;i++)
	{
		APList[i]->SetVisibility(ESlateVisibility::Visible);
	}
	AbilityName->SetText(Name);
	AbilityName->SetVisibility(ESlateVisibility::Visible);
	AbilityIcon->SetVisibility(ESlateVisibility::Visible);
}

void UCmdCell::DoSelected()
{
	PlayAnimation(Selected);
}

void UCmdCell::DoUnSelected()
{
	PlayAnimation(UnSelected);
}

void UCmdCell::HideContent()
{
	for(int i = 0;i < APList.Num();i++)
	{
		APList[i]->SetVisibility(ESlateVisibility::Hidden);
	}
	AbilityName->SetVisibility(ESlateVisibility::Hidden);
	AbilityIcon->SetVisibility(ESlateVisibility::Hidden);
}

