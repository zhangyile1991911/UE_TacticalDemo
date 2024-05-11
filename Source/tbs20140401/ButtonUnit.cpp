// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonUnit.h"

#include "My_Utilities.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"

void UButtonUnit::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	RefreshButtonStyle();
}

void UButtonUnit::NativeConstruct()
{
	Super::NativeConstruct();
	UnitButton->OnClicked.AddDynamic(this,&UButtonUnit::OnUnitButtonClicked);
}

void UButtonUnit::RefreshButtonStyle()const
{
	auto pData = GetUnitData(UnitType);
	if(pData == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("%d is nullptr"),UnitType);
		return;
	}
	if(pData->Assets.Icon.IsValid())
	{
		UnitImage->SetBrushFromTexture(pData->Assets.Icon.Get());	
	}
	else
	{
		auto iconData = pData->Assets.Icon.LoadSynchronous();
		UnitImage->SetBrushFromTexture(iconData);
	}

	if(IsLookingRight)
	{
		UnitImage->SetRenderScale(FVector2D(1,1));	
	}
	else
	{
		UnitImage->SetRenderScale(FVector2D(-1,1));
	}

	Border_IsSelected->SetBrushColor(IsSelected ? FLinearColor::Blue : FLinearColor::White);
}

void UButtonUnit::OnUnitButtonClicked()
{
	if(OnClickTopic.IsBound())
		OnClickTopic.Execute(this);
}

void UButtonUnit::SetIsSelected(bool selected)
{
	IsSelected = selected;
	RefreshButtonStyle();
}
