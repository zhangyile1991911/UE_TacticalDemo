// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstRolePortrait.h"

#include "MyUnit.h"
#include "My_Utilities.h"
#include "MyGameInstance.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UFirstRolePortrait::RefreshByUnit(TObjectPtr<AMyUnit> Unit)
{
	if(GameInstance == nullptr)
	{
		GameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	}
	auto pData = GameInstance->GetUnitData(Unit->GetUnitType());
	if(pData == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("%d data is null"),Unit->GetUnitType());
		return;
	}
	if(!pData->Assets.Icon.IsValid())
	{
		pData->Assets.Icon.LoadSynchronous();
	}
	Portrait->SetBrushFromTexture(pData->Assets.Icon.Get(),false);

	switch (Unit->GetUnitType())
	{
	case ETBSUnitType::Warrior:
		Name->SetText(FText::FromName(TEXT("战士")));
		break;
	case ETBSUnitType::Slime:
		Name->SetText(FText::FromName(TEXT("史莱姆")));
		break;
	case ETBSUnitType::Ranger:
		Name->SetText(FText::FromName(TEXT("游侠")));
		break;
	}
	
	for(auto one : APs)
	{
		one->SetBrushTintColor(FLinearColor::Gray);
	}

	FSlateColor bright(FLinearColor(0.904661f,0.806952f,0.078187f,1.0f));
	for(int i = 0;i < Unit->GetRuntimeProperty().ActionPoint;i++)
	{
		APs[i]->SetBrushTintColor(bright);
	}

	int CurHP = Unit->GetRuntimeProperty().HP;
	int ConfigHP = Unit->GetProperty().HP;
	FText str = FText::Format(FormatTemplate,CurHP,ConfigHP);
	HPText->SetText(str);

	float per = static_cast<float>(CurHP)/static_cast<float>(ConfigHP);
	HPBar->SetPercent(per);
}

void UFirstRolePortrait::NativeConstruct()
{
	Super::NativeConstruct();
	APs.Reserve(3);

	APs.Add(AP1);
	APs.Add(AP2);
	APs.Add(AP3);
}

void UFirstRolePortrait::NativeDestruct()
{
	Super::NativeDestruct();
	APs.Empty();
}
