// Fill out your copyright notice in the Description page of Project Settings.


#include "UGameUI_UnitBreifInfo.h"

#include "BattleFunc.h"
#include "MyButtonList_Units.h"
#include "Components/TextBlock.h"
#include "MyUnit.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
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


void UUGameUI_UnitBriefInfo::RefreshUnitBriefInfo(TObjectPtr<AMyUnit> Attacker,TObjectPtr<AMyUnit> Defender,float HitPercent)
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
	
	for(int  i = 0;i < 3;i++)
	{
		APList[i]->SetVisibility(ESlateVisibility::Visible);
	}

	if(Attacker == nullptr)
	{
		HitPerText->SetVisibility(ESlateVisibility::Hidden);	
	}
	else
	{
		HitPerText->SetVisibility(ESlateVisibility::Visible);
		FText TmpHit = FText::Format(FText::FromString("{0}%"), FText::AsNumber(HitPercent));
		HitPerText->SetText(TmpHit);
		
	}

	const float fHP = Defender->GetRuntimeProperty().HP;
	const float fHPConfig = Defender->GetProperty().HP;
	HPBar->SetPercent(fHP/fHPConfig);
	
	CurHPText->SetText(FText::FromString(FString::FormatAsNumber(fHP)));
	MaxHPText->SetText(FText::FromString(FString::FormatAsNumber(fHPConfig)));
}

void UUGameUI_UnitBriefInfo::ShowMoveOnly()
{
	InfoNode->SetVisibility(ESlateVisibility::Hidden);
	TabNode->SetVisibility(ESlateVisibility::Hidden);
	ConfirmNode->SetVisibility(ESlateVisibility::Visible);
}

void UUGameUI_UnitBriefInfo::ShowDetailOnly()
{
	InfoNode->SetVisibility(ESlateVisibility::Hidden);
	ConfirmNode->SetVisibility(ESlateVisibility::Hidden);
	TabNode->SetVisibility(ESlateVisibility::Visible);
	TabText->SetText(FText::FromName(TEXT("详情")));
}

void UUGameUI_UnitBriefInfo::ShowSelfCmd(TObjectPtr<AMyUnit> Attacker)
{
	InfoNode->SetVisibility(ESlateVisibility::Hidden);
	TabNode->SetVisibility(ESlateVisibility::Visible);
	ConfirmNode->SetVisibility(ESlateVisibility::Visible);
	ConfirmText->SetText(FText::FromName(TEXT("选择指令")));
	TabText->SetText(FText::FromName(TEXT("详情")));
}

void UUGameUI_UnitBriefInfo::ShowConfirmCmd(const FText& ConfirmTxt)
{
	InfoNode->SetVisibility(ESlateVisibility::Hidden);
	TabNode->SetVisibility(ESlateVisibility::Hidden);
	ConfirmNode->SetVisibility(ESlateVisibility::Visible);
	ConfirmText->SetText(ConfirmTxt);
}

void UUGameUI_UnitBriefInfo::ShowTarget(TObjectPtr<AMyUnit> Attacker,TObjectPtr<AMyUnit> Defender,float HitPercent,FText ConfirmTxt,FText DetailTxt)
{
	InfoNode->SetVisibility(ESlateVisibility::Visible);
	TabNode->SetVisibility(ESlateVisibility::Visible);
	ConfirmNode->SetVisibility(ESlateVisibility::Visible);
	
	ConfirmText->SetText(ConfirmTxt);
	
	TabText->SetText(DetailTxt);
	RefreshUnitBriefInfo(Attacker,Defender,HitPercent);
}


void UUGameUI_UnitBriefInfo::ShowTargetInfoAndTab(TObjectPtr<AMyUnit> Defender,float HitPercent)
{
	InfoNode->SetVisibility(ESlateVisibility::Visible);
	TabNode->SetVisibility(ESlateVisibility::Visible);
	ConfirmNode->SetVisibility(ESlateVisibility::Hidden);

	TabText->SetText(FText::FromName(TEXT("详情")));
	RefreshUnitBriefInfo(nullptr,Defender,HitPercent);
}

void UUGameUI_UnitBriefInfo::ShowTargetInfoAndConfirmAndTab(TObjectPtr<AMyUnit> Defender,FText ConfirmTxt,FText TabTxt)
{
	InfoNode->SetVisibility(ESlateVisibility::Visible);
	TabNode->SetVisibility(ESlateVisibility::Visible);
	ConfirmNode->SetVisibility(ESlateVisibility::Visible);

	ConfirmText->SetText(ConfirmTxt);
	TabText->SetText(TabTxt);
	
	RefreshUnitBriefInfo(nullptr,Defender,0);
}



