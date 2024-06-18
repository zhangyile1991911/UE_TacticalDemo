// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitInfoDetail.h"

#include "MyUnit.h"
#include "My_Utilities.h"
#include "UnitAbilityAnim.h"
#include "UnitInfoDetailPortrait.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
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

	for(int i = 0;i < 5;i++)
	{
		if(!PortraitClassPtr.IsValid())
		{
			PortraitClassPtr.LoadSynchronous();
		}
		const auto Widget = CreateWidget(this,PortraitClassPtr.Get());
		PortraitBox->AddChild(Widget);
		
		auto Portrait = Cast<UUnitInfoDetailPortrait>(Widget);
		PortraitPool.Add(Portrait);	
	}
	ArrayOfUnitTeam.Reserve(8);
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
		JobTxt->SetText(NSLOCTEXT("","","战士"));
		break;
	case EUnitType::Ranger:
		JobTxt->SetText(NSLOCTEXT("","","游侠"));
		break;
	case EUnitType::Slime:
		JobTxt->SetText(NSLOCTEXT("","","史莱姆"));
		break;
	}

	HPProgress->SetPercent(float(TableProperty.HP)/float(RuntimeProperty.HP));

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
	int i = 0;
	for(;i < AbilityList.Num();i++)
	{
		ArrayOfSkillImg[i]->SetVisibility(ESlateVisibility::Visible);
		ArrayOfSkillName[i]->SetVisibility(ESlateVisibility::Visible);
		ArrayOfSkillName[i]->SetText(FText::Format(NSLOCTEXT("","","{0}"),AbilityList[i]->GetSkillData().SkillName));
	}
	for(;i < ArrayOfSkillImg.Num();i++)
	{
		ArrayOfSkillImg[i]->SetVisibility(ESlateVisibility::Hidden);
		ArrayOfSkillName[i]->SetVisibility(ESlateVisibility::Hidden);
	}

	EsotericImg->SetVisibility(ESlateVisibility::Collapsed);
	EsotericName->SetVisibility(ESlateVisibility::Collapsed);
}

void UUnitInfoDetail::ShowUnitTeamInfo(TArray<TObjectPtr<AMyUnit>> UnitTeam,TObjectPtr<AMyUnit> FocusUnit)
{
	ArrayOfUnitTeam = UnitTeam;
	if(ArrayOfUnitTeam.Num() > PortraitPool.Num())
	{
		int delta = ArrayOfUnitTeam.Num() - PortraitPool.Num();
		for(int i = 0;i < delta;i++)
		{
			auto Widget = CreateWidget(PortraitBox,PortraitClassPtr.Get());
        	auto Portrait = Cast<UUnitInfoDetailPortrait>(Widget);
        	PortraitPool.Add(Portrait);	
		}
	}

	int i = 0;
	for(;i < ArrayOfUnitTeam.Num();i++)
	{
		PortraitPool[i]->ShowUnitPortrait(ArrayOfUnitTeam[i]->GetUnitType());
	}

	for(;i < PortraitPool.Num();i++)
	{
		PortraitPool[i]->HideUnitPortrait();
	}
	SetVisibility(ESlateVisibility::Visible);
}
