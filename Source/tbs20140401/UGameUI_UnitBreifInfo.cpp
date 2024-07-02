﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UGameUI_UnitBreifInfo.h"

#include "BattleFunc.h"
#include "MyButtonList_Units.h"
#include "Components/TextBlock.h"
#include "MyUnit.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"

void UUGameUI_UnitBriefInfo::NativeConstruct()
{
	Super::NativeConstruct();

	TabMoveNode->SetVisibility(ESlateVisibility::Collapsed);
	APList.Add(AP1);
	APList.Add(AP2);
	APList.Add(AP3);
}

void UUGameUI_UnitBriefInfo::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUGameUI_UnitBriefInfo::RefreshUnitBaseBriefInfo(TObjectPtr<AMyUnit> Unit)
{
	//Level
	FText TmpLevel = FText::Format(FText::FromString("LV.{0}"), FText::AsNumber(1));
	LevelText->SetText(TmpLevel);
	//Job
	switch (Unit->GetUnitType())
	{
	case EUnitType::Warrior:
	case EUnitType::EnemyWarrior:
		UnitNameText->SetText(FText::FromName(TEXT("戦士")));
		break;
	case EUnitType::Slime:
	case EUnitType::EnemySlime:
		UnitNameText->SetText(FText::FromName(TEXT("スライム")));
		break;
	case EUnitType::Ranger:
	case EUnitType::EnemyRanger:
		UnitNameText->SetText(FText::FromName(TEXT("レンジャー")));
		break;
	case EUnitType::Bat:
	case EUnitType::EnemyBat:
		UnitNameText->SetText(FText::FromName(TEXT("バット")));
		break;
	case EUnitType::Chicken:
	case EUnitType::EnemyChicken:
		UnitNameText->SetText(FText::FromName(TEXT("鳥")));
		break;
	case EUnitType::Priest:
	case EUnitType::EnemyPriest:
		UnitNameText->SetText(FText::FromName(TEXT("司祭")));
		break;
	case EUnitType::EnemyTank:
		UnitNameText->SetText(FText::FromName(TEXT("タンク")));
		break;
	}
	//AP
	for(int  i = 0;i < 3;i++)
	{
		APList[i]->SetVisibility(ESlateVisibility::Visible);
	}
	//HP
	const float fHP = Unit->GetRuntimeProperty().HP;
	const float fHPConfig = Unit->GetProperty().HP;
	HPBar->SetPercent(fHP/fHPConfig);
	
	CurHPText->SetText(FText::FromString(FString::FormatAsNumber(fHP)));
	MaxHPText->SetText(FText::FromString(FString::FormatAsNumber(fHPConfig)));
}


void UUGameUI_UnitBriefInfo::RefreshUnitBriefInfo(TObjectPtr<AMyUnit> Attacker,TObjectPtr<AMyUnit> Defender,float HitPercent)
{
	FText TmpLevel = FText::Format(FText::FromString("LV.{0}"), FText::AsNumber(1));
	LevelText->SetText(TmpLevel);

	switch (Defender->GetUnitType())
	{
	case EUnitType::Warrior:
	case EUnitType::EnemyWarrior:
		UnitNameText->SetText(FText::FromName(TEXT("戦士")));
		break;
	case EUnitType::Slime:
	case EUnitType::EnemySlime:
		UnitNameText->SetText(FText::FromName(TEXT("スライム")));
		break;
	case EUnitType::Ranger:
	case EUnitType::EnemyRanger:
		UnitNameText->SetText(FText::FromName(TEXT("レンジャー")));
		break;
	case EUnitType::Bat:
	case EUnitType::EnemyBat:
		UnitNameText->SetText(FText::FromName(TEXT("バット")));
		break;
	case EUnitType::Chicken:
	case EUnitType::EnemyChicken:
		UnitNameText->SetText(FText::FromName(TEXT("鳥")));
		break;
	case EUnitType::Priest:
	case EUnitType::EnemyPriest:
		UnitNameText->SetText(FText::FromName(TEXT("司祭")));
		break;
	case EUnitType::EnemyTank:
		UnitNameText->SetText(FText::FromName(TEXT("タンク")));
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

void UUGameUI_UnitBriefInfo::ShowDetailTabOnly(const FVector& Location)
{
	InfoNode->SetVisibility(ESlateVisibility::Hidden);
	ConfirmNode->SetVisibility(ESlateVisibility::Hidden);
	TabNode->SetVisibility(ESlateVisibility::Visible);

	UpdateWidgetPosition(Location);
	// TabText->SetText(FText::FromName(TEXT("詳細")));
}

void UUGameUI_UnitBriefInfo::ShowSelfCmd(TObjectPtr<AMyUnit> Attacker)
{
	TabNode->SetVisibility(ESlateVisibility::Visible);
	CmdNode->SetVisibility(ESlateVisibility::Visible);
	
	ConfirmNode->SetVisibility(ESlateVisibility::Collapsed);
	InfoNode->SetVisibility(ESlateVisibility::Collapsed);
	TabMoveNode->SetVisibility(ESlateVisibility::Collapsed);
	UpdateWidgetPosition(Attacker->GetActorLocation());
	// ConfirmText->SetText(FText::FromName(TEXT("コマンド選択")));
	// TabText->SetText(FText::FromName(TEXT("詳細")));
}

void UUGameUI_UnitBriefInfo::ShowConfirmCmd(const FVector& Location)
{
	InfoNode->SetVisibility(ESlateVisibility::Hidden);
	TabNode->SetVisibility(ESlateVisibility::Hidden);
	ConfirmNode->SetVisibility(ESlateVisibility::Visible);
	// UpdateWidgetPosition(Defender->GetActorLocation());
	// ConfirmText->SetText(ConfirmTxt);
	UpdateWidgetPosition(Location);
}

void UUGameUI_UnitBriefInfo::ShowTarget(TObjectPtr<AMyUnit> Attacker,TObjectPtr<AMyUnit> Defender,float HitPercent)
{
	InfoNode->SetVisibility(ESlateVisibility::Visible);
	TabNode->SetVisibility(ESlateVisibility::Visible);
	ConfirmNode->SetVisibility(ESlateVisibility::Visible);

	TabMoveNode->SetVisibility(ESlateVisibility::Collapsed);
	CmdNode->SetVisibility(ESlateVisibility::Collapsed);
	
	// ConfirmText->SetText(ConfirmTxt);
	//
	// TabText->SetText(DetailTxt);
	RefreshUnitBriefInfo(Attacker,Defender,HitPercent);
	UpdateWidgetPosition(Defender->GetActorLocation());
}


void UUGameUI_UnitBriefInfo::ShowTargetInfoAndTab(TObjectPtr<AMyUnit> Defender,float HitPercent)
{
	InfoNode->SetVisibility(ESlateVisibility::Visible);
	TabNode->SetVisibility(ESlateVisibility::Visible);
	ConfirmNode->SetVisibility(ESlateVisibility::Visible);

	CmdNode->SetVisibility(ESlateVisibility::Collapsed);
	TabMoveNode->SetVisibility(ESlateVisibility::Collapsed);
	// TabText->SetText(FText::FromName(TEXT("詳細")));
	RefreshUnitBriefInfo(nullptr,Defender,HitPercent);
	UpdateWidgetPosition(Defender->GetActorLocation());
}

void UUGameUI_UnitBriefInfo::ShowTargetInfoAndConfirmAndTab(TObjectPtr<AMyUnit> Defender)
{
	InfoNode->SetVisibility(ESlateVisibility::Visible);
	TabNode->SetVisibility(ESlateVisibility::Visible);
	ConfirmNode->SetVisibility(ESlateVisibility::Visible);

	TabMoveNode->SetVisibility(ESlateVisibility::Collapsed);
	CmdNode->SetVisibility(ESlateVisibility::Collapsed);
	// ConfirmText->SetText(ConfirmTxt);
	// TabText->SetText(TabTxt);
	
	RefreshUnitBriefInfo(nullptr,Defender,0);
	UpdateWidgetPosition(Defender->GetActorLocation());
}

TObjectPtr<UCanvasPanelSlot> UUGameUI_UnitBriefInfo::GetCanvasPanelSlot()
{
	if(CanvasPanelSlot == nullptr)
	{
		CanvasPanelSlot = Cast<UCanvasPanelSlot>(this->Slot);	
	}
	return CanvasPanelSlot;
}

void UUGameUI_UnitBriefInfo::UpdateWidgetPosition(const FVector& Location)
{
	if(PlayerControllerPtr == nullptr)
	{
		PlayerControllerPtr = GetWorld()->GetFirstPlayerController();	
	}
	
	FVector2D ScreenLocation;
	const bool Result = PlayerControllerPtr->ProjectWorldLocationToScreen(Location,ScreenLocation,false);
	if(Result)
	{
		const float Scale = UWidgetLayoutLibrary::GetViewportScale(PlayerControllerPtr);
		ScreenLocation /= Scale;
		UCanvasPanelSlot* CanvasSlot = GetCanvasPanelSlot();
		CanvasSlot->SetPosition(ScreenLocation);
	}
}

void UUGameUI_UnitBriefInfo::ShowMoveOnly(const FVector& Location)
{
	TabMoveNode->SetVisibility(ESlateVisibility::Visible);

	ConfirmNode->SetVisibility(ESlateVisibility::Collapsed);
	InfoNode->SetVisibility(ESlateVisibility::Collapsed);
	TabNode->SetVisibility(ESlateVisibility::Collapsed);
	CmdNode->SetVisibility(ESlateVisibility::Collapsed);

	UpdateWidgetPosition(Location);
}

void UUGameUI_UnitBriefInfo::ShowDetailInfoOnly(TObjectPtr<AMyUnit> Unit)
{
	InfoNode->SetVisibility(ESlateVisibility::Visible);
	
	TabMoveNode->SetVisibility(ESlateVisibility::Collapsed);
	ConfirmNode->SetVisibility(ESlateVisibility::Collapsed);
	TabNode->SetVisibility(ESlateVisibility::Collapsed);
	CmdNode->SetVisibility(ESlateVisibility::Collapsed);

	RefreshUnitBaseBriefInfo(Unit);
	HitIcon->SetVisibility(ESlateVisibility::Collapsed);
	HitPerText->SetVisibility(ESlateVisibility::Collapsed);
	UpdateWidgetPosition(Unit->GetActorLocation());
}



