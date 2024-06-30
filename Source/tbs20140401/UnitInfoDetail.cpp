// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitInfoDetail.h"

#include "MyUnit.h"
#include "My_Utilities.h"
#include "UnitAbilityAnim.h"
#include "UnitInfoDetailPortrait.h"
#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/Spacer.h"
#include "Components/TextBlock.h"

void UUnitInfoDetail::NativeConstruct()
{
	Super::NativeConstruct();
	ArrayOfSkillImg.Reserve(8);
	ArrayOfSkillName.Reserve(8);

	ArrayOfSkillImg.Add(SKillImgA);
	ArrayOfSkillImg.Add(SKillImgB);
	ArrayOfSkillImg.Add(SKillImgC);
	ArrayOfSkillImg.Add(SKillImgD);
	ArrayOfSkillImg.Add(SKillImgE);
	ArrayOfSkillImg.Add(SKillImgF);
	ArrayOfSkillImg.Add(SKillImgG);

	ArrayOfSkillName.Add(SKillNameA);
	ArrayOfSkillName.Add(SKillNameB);
	ArrayOfSkillName.Add(SKillNameC);
	ArrayOfSkillName.Add(SKillNameD);
	ArrayOfSkillName.Add(SKillNameE);
	ArrayOfSkillName.Add(SKillNameF);
	ArrayOfSkillName.Add(SKillNameG);

	PortraitPool.Add(P1);
	PortraitPool.Add(P2);
	PortraitPool.Add(P3);
	PortraitPool.Add(P4);
	PortraitPool.Add(P5);
	PortraitPool.Add(P6);
	PortraitPool.Add(P7);
	PortraitPool.Add(P8);
	PortraitPool.Add(P9);
	PortraitPool.Add(P10);

	for(int i = 0;i < PortraitPool.Num();i++)
	{
		PortraitPool[i]->SetVisibility(ESlateVisibility::Collapsed);
	}
	// for(int i = 0;i < 5;i++)
	// {
	// 	if(!PortraitClassPtr.IsValid())
	// 	{
	// 		PortraitClassPtr.LoadSynchronous();
	// 	}
	// 	const auto Widget = CreateWidget(this,PortraitClassPtr.Get());
	//
	// 	// if(i <= 4)
	// 	// 	Widget->SetPadding(FMargin(0,0,10,0));
	// 	
	// 	// PortraitBox->InsertChildAt(1+i,Widget);
	// 	// PortraitBox->InsertChildAt(1,Widget);
	// 	PortraitBox->AddChild(Widget);
	// 	auto Portrait = Cast<UUnitInfoDetailPortrait>(Widget);
	// 	PortraitPool.Add(Portrait);	
	// }
	
}

void UUnitInfoDetail::NativeDestruct()
{
	Super::NativeDestruct();
	for(int i = 0; i < PortraitPool.Num();i++)
	{
		PortraitBox->RemoveChild(PortraitPool[i]);	
	}
	PortraitPool.Empty();
}

void UUnitInfoDetail::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UUnitInfoDetail::ShowUnitDetailInfo(AMyUnit* MyUnit)
{
	if(MyUnit == nullptr)return;

	const auto& RuntimeProperty = MyUnit->GetRuntimeProperty();
	const auto& TableProperty = MyUnit->GetProperty();
	
	LevelTxt->SetText(FText::Format(NSLOCTEXT("","","等级{0}"),TableProperty.Level));
	
	NameTxt->SetText(TableProperty.UnitName);
	
	switch (MyUnit->GetUnitType())
	{
	case EUnitType::Warrior:
	case EUnitType::EnemyWarrior:
		JobTxt->SetText(NSLOCTEXT("","","战士"));
		break;
	case EUnitType::Ranger:
	case EUnitType::EnemyRanger:
		JobTxt->SetText(NSLOCTEXT("","","游侠"));
		break;
	case EUnitType::Slime:
	case EUnitType::EnemySlime:
		JobTxt->SetText(NSLOCTEXT("","","史莱姆"));
		break;
	case EUnitType::Priest:
	case EUnitType::EnemyPriest:
		JobTxt->SetText(NSLOCTEXT("","","牧师"));
		break;
	case EUnitType::Chicken:
	case EUnitType::EnemyChicken:
		JobTxt->SetText(NSLOCTEXT("","","鸡"));
		break;
	case EUnitType::Bat:
	case EUnitType::EnemyBat:
		JobTxt->SetText(NSLOCTEXT("","","蝙蝠"));
		break;
	case EUnitType::EnemyTank:
		JobTxt->SetText(NSLOCTEXT("","","坦克"));
		break;
	}

	HPProgress->SetPercent(float(RuntimeProperty.HP)/float(TableProperty.HP));
	HPTxt->SetText(FText::Format(NSLOCTEXT("","","{0}/{1}"),RuntimeProperty.HP,TableProperty.HP));

	//武器
	const auto WeaponDataPtr = GetWeaponData(TableProperty.WeaponId);
	WeaponName->SetText(FText::FromName(WeaponDataPtr->WeaponName));
	
	AccessoryImgA->SetVisibility(ESlateVisibility::Hidden);
	AccessoryNameA->SetVisibility(ESlateVisibility::Hidden);

	AccessoryImgB->SetVisibility(ESlateVisibility::Hidden);
	AccessoryNameB->SetVisibility(ESlateVisibility::Hidden);

	PowerTxt->SetText(FText::Format(NSLOCTEXT("","","{0}"),RuntimeProperty.Power));
	PhysicalDefNum->SetText(FText::Format(NSLOCTEXT("","","{0}"),RuntimeProperty.PhysicDefend));
	MagicNum->SetText(FText::Format(NSLOCTEXT("","","{0}"),RuntimeProperty.MagicPower));
	MagicDefNum->SetText(FText::Format(NSLOCTEXT("","","{0}"),RuntimeProperty.MagicDefend));
	LuckyNum->SetText(FText::Format(NSLOCTEXT("","","{0}"),RuntimeProperty.Lucky));
	HitNum->SetText(FText::Format(NSLOCTEXT("","","{0}"),RuntimeProperty.HitProb));
	SpedNum->SetText(FText::Format(NSLOCTEXT("","","{0}"),RuntimeProperty.Speed));
	DodgeNum->SetText(FText::Format(NSLOCTEXT("","","{0}"),RuntimeProperty.Dodge));
	JumpNum->SetText(FText::Format(NSLOCTEXT("","","{0}"),RuntimeProperty.Jump));
	MoveNum->SetText(FText::Format(NSLOCTEXT("","","{0}"),RuntimeProperty.Move));

	const auto& AbilityList = MyUnit->GetOwnAbilityList();
	int AbilityCount = 0;
	for(int i = 0;i < AbilityList.Num();i++)
	{
		if(AbilityList[i]->IsIdle())continue;
		if(!AbilityList[i]->IsShowOnCmd())continue;
		ArrayOfSkillImg[i]->SetVisibility(ESlateVisibility::Visible);
		ArrayOfSkillName[i]->SetVisibility(ESlateVisibility::Visible);
		ArrayOfSkillName[i]->SetText(FText::Format(NSLOCTEXT("","","{0}"),AbilityList[i]->GetSkillData().SkillName));
		AbilityCount++;
	}
	for(int i = AbilityCount;i < ArrayOfSkillImg.Num();i++)
	{
		ArrayOfSkillImg[i]->SetVisibility(ESlateVisibility::Hidden);
		ArrayOfSkillName[i]->SetVisibility(ESlateVisibility::Hidden);
	}

	EsotericImg->SetVisibility(ESlateVisibility::Collapsed);
	EsotericName->SetVisibility(ESlateVisibility::Collapsed);

	const FUnitData* UnitData = GetUnitData(MyUnit->GetUnitType());
	HalfPortrait->SetBrushFromTexture(UnitData->Assets.Icon.Get());
	
}

void UUnitInfoDetail::ShowUnitTeamInfo(TArray<TObjectPtr<AMyUnit>> UnitTeam,TObjectPtr<AMyUnit> FocusUnit)
{
	ArrayOfUnitTeam = UnitTeam;
	
	// if(ArrayOfUnitTeam.Num() > PortraitPool.Num())
	// {
	// 	int delta = ArrayOfUnitTeam.Num() - PortraitPool.Num();
	// 	int InsertIndex = PortraitPool.Num();
	// 	for(int i = 0;i < delta;i++)
	// 	{
	// 		auto Widget = CreateWidget(PortraitBox,PortraitClassPtr.Get());
	// 		if(i <= delta - 1)
	// 			Widget->SetPadding(FMargin(0,0,10,0));
 //        	auto Portrait = Cast<UUnitInfoDetailPortrait>(Widget);
 //        	PortraitPool.Add(Portrait);
	// 		PortraitBox->InsertChildAt(InsertIndex+i,Portrait);
	// 	}
	// }

	int i = 0;
	for(;i < ArrayOfUnitTeam.Num();i++)
	{
		PortraitPool[i]->ShowUnitPortrait(ArrayOfUnitTeam[i]->GetUnitType());
	}

	for(;i < PortraitPool.Num();i++)
	{
		PortraitPool[i]->HideUnitPortrait();
	}
	for(int x = 0;x < UnitTeam.Num();x++)
	{
		if(UnitTeam[x]->GetUniqueID() == FocusUnit->GetUniqueID())
		{
			FocusIndex = x;
			PortraitPool[x]->DoHighlight();
			break;
		}
	}
	ShowUnitDetailInfo(FocusUnit);
	SetVisibility(ESlateVisibility::Visible);
}

void UUnitInfoDetail::HideUnitTeamInfo()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UUnitInfoDetail::NextUnit()
{
	int NewIndex = FocusIndex + 1;
	if(NewIndex >= ArrayOfUnitTeam.Num())
	{
		NewIndex = 0;
	}
	
	ShowUnitDetailInfo(ArrayOfUnitTeam[NewIndex]);

	PortraitPool[FocusIndex]->DoUnhighlight();
	FocusIndex = NewIndex;
	PortraitPool[FocusIndex]->DoHighlight();
}

void UUnitInfoDetail::PreviousUnit()
{
	int NewIndex = FocusIndex - 1;
	if(NewIndex < 0)
	{
		NewIndex = ArrayOfUnitTeam.Num() - 1;
	}

	ShowUnitDetailInfo(ArrayOfUnitTeam[NewIndex]);

	PortraitPool[FocusIndex]->DoUnhighlight();
	FocusIndex = NewIndex;
	PortraitPool[FocusIndex]->DoHighlight();
}
