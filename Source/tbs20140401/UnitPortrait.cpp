// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitPortrait.h"
#include "My_Utilities.h"
#include "MyUnit.h"
#include "Components/Image.h"

void UUnitPortrait::RefreshUnitIcon(TObjectPtr<AMyUnit> Unit)
{
	auto pData = GetUnitData(Unit->GetUnitType());
	if(pData == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("%d data is null"),Unit->GetUnitType());
		return;
	}
	Portrait->SetBrushFromTexture(pData->Assets.Icon.Get(),false);

	for(auto one : APs)
	{
		one->SetBrushTintColor(FLinearColor::Gray);
	}

	FSlateColor bright(FLinearColor(0.904661f,0.806952f,0.078187f,1.0f));
	for(int i = 0;i < Unit->GetProperty().SkillPoint;i++)
	{
		APs[i]->SetBrushTintColor(bright);
	}
}

void UUnitPortrait::NativeConstruct()
{
	Super::NativeConstruct();

	APs.Add(AP1);
	APs.Add(AP2);
	APs.Add(AP3);
}

void UUnitPortrait::NativeDestruct()
{
	APs.Empty();
	Super::NativeDestruct();
}
