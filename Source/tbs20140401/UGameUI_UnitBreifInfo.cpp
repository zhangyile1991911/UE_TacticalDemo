// Fill out your copyright notice in the Description page of Project Settings.


#include "UGameUI_UnitBreifInfo.h"

#include "MyButtonList_Units.h"
#include "Components/TextBlock.h"
#include "MyUnit.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"

void UUGameUI_UnitBriefInfo::NativeConstruct()
{
	Super::NativeConstruct();

	APList.Add(AP1);
	APList.Add(AP2);
	APList.Add(AP3);
}

void UUGameUI_UnitBriefInfo::NativeDestruct()
{
	Super::NativeDestruct();
}


void UUGameUI_UnitBriefInfo::RefreshUnitBriefInfo(TObjectPtr<AMyUnit> Attacker,TObjectPtr<AMyUnit> Defender)
{
	FText TmpLevel = FText::Format(FText::FromString("等级 {0}"), FText::AsNumber(1));
	LevelText->SetText(TmpLevel);

	switch (Defender->GetUnitType())
	{
	case EUnitType::Warrior:
		UnitNameText->SetText(FText::FromName(TEXT("战士")));
		break;
	case EUnitType::Slime:
		UnitNameText->SetText(FText::FromName(TEXT("史莱姆")));
		break;
	case EUnitType::Ranger:
		UnitNameText->SetText(FText::FromName(TEXT("游侠")));
		break;
	}

	//todo 等运行时候属性设计完成
	for(int  i = 0;i < 3;i++)
	{
		APList[i]->SetVisibility(ESlateVisibility::Visible);
	}

	FText TmpHit = FText::Format(FText::FromString("{0}%"), FText::AsNumber(22));
	HitPerText->SetText(TmpHit);
	
	HPBar->SetPercent(0.65f);
}

