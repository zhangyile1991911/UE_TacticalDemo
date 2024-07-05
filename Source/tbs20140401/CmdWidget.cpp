// Fill out your copyright notice in the Description page of Project Settings.


#include "CmdWidget.h"

#include "CmdCell.h"
#include "MyUnit.h"
#include "SkillData.h"
#include "UnitAbilityAnim.h"
#include "Components/TextBlock.h"

void UCmdWidget::NativeConstruct()
{
	Super::NativeConstruct();
	AbilityList.Add(AbilityCellA);
	AbilityList.Add(AbilityCellB);
	AbilityList.Add(AbilityCellC);
	AbilityList.Add(AbilityCellD);
	AbilityList.Add(AbilityCellE);
	AbilityList.Add(AbilityCellF);
	AbilityList.Add(AbilityCellG);
}

void UCmdWidget::RefreshUnitCmd(TObjectPtr<AMyUnit> Unit,bool bShowIdle)
{
	ArrayOfAbility = Unit->GetOwnAbilityList();
	AbilityNum = 0;
	int i = 0;
	for(;i < ArrayOfAbility.Num();i++)
	{
		if(ArrayOfAbility[i]->IsShowOnCmd() == false)continue;
		if(ArrayOfAbility[i]->IsIdle() && !bShowIdle)continue;
		
		AbilityList[i]->RefreshCell(ArrayOfAbility[i]->GetAbilityName(),ArrayOfAbility[i]->GetCost());
		AbilityList[i]->SetVisibility(ESlateVisibility::Visible);
		AbilityNum++;
	}
	i = AbilityNum;
	for(;i < AbilityList.Num();i++)
	{
		// AbilityList[i]->SetVisibility(ESlateVisibility::Hidden);
		AbilityList[i]->HideContent();
	}
}

void UCmdWidget::SelectCmd(int index)
{
	if(index < 0 || index >= AbilityNum)return;
	if(index == SelectedIndex)return;
	
	if(SelectedIndex >= 0 && SelectedIndex < AbilityNum)
		AbilityList[SelectedIndex]->DoUnSelected();

	AbilityList[index]->DoSelected();
	SelectedIndex = index;

	if(ArrayOfAbility[index]->IsIdle())
	{
		DamageTitle->SetText(FText::FromName("-"));
		DamageNum->SetText(FText::FromName("-"));
		RangeTitle->SetText(FText::FromName("-"));
		RangeNum->SetText(FText::FromName("-"));
	}
	else
	{
		DamageTitle->SetText(FText::FromString(TEXT("威力")));
		RangeTitle->SetText(FText::FromString(TEXT("射程")));
		DamageNum->SetText(FText::Format(NSLOCTEXT("","","{0}"),10));
		RangeNum->SetText(FText::Format(NSLOCTEXT("","","{0}~{1} (高度-{2}~+{3})}"),
			ArrayOfAbility[index]->GetSkillData().Range.X,
			ArrayOfAbility[index]->GetSkillData().Range.Y,
			ArrayOfAbility[index]->GetSkillData().AllowableDeviation,
			ArrayOfAbility[index]->GetSkillData().AllowableDeviation));
	}
	Describe->SetText(ArrayOfAbility[index]->GetSkillData().Description);	
	
}

void UCmdWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	for (TObjectPtr<UCmdCell>& AbilityCell : AbilityList)
	{
		AbilityCell = nullptr;
	}
	
	AbilityList.Empty();
}
