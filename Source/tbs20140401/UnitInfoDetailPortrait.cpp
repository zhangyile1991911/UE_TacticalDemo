// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitInfoDetailPortrait.h"
#include "My_Utilities.h"
#include "Components/Image.h"

void UUnitInfoDetailPortrait::ShowUnitPortrait(EUnitType UnitType)
{
	const FUnitData* UnitData = GetUnitData(UnitType);
	if(UnitData == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("%d data is null"),UnitType);
		return;
	}

	if(UnitData->Assets.Icon.IsValid())
	{
		PortraitImage->SetBrushFromTexture(UnitData->Assets.Icon.Get());
	}
	SetVisibility(ESlateVisibility::Visible);
}

void UUnitInfoDetailPortrait::HideUnitPortrait()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UUnitInfoDetailPortrait::DoHighlight()
{
	PlayAnimation(Highlight);
}

void UUnitInfoDetailPortrait::DoUnhighlight()
{
	PlayAnimation(UnHighlight);
}
