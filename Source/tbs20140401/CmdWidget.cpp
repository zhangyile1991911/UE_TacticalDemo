// Fill out your copyright notice in the Description page of Project Settings.


#include "CmdWidget.h"

#include "CmdCell.h"
#include "MyUnit.h"
// #include "UnitAbility.h"
#include "UnitAbilityAnim.h"
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

void UCmdWidget::RefreshUnitCmd(TObjectPtr<AMyUnit> Unit)
{
	const TArray<TObjectPtr<AUnitAbilityAnim>>& UnitAbilities = Unit->GetOwnAbilityList();
	AbilityNum = 0;
	int i = 0;
	for(;i < UnitAbilities.Num();i++)
	{
		if(UnitAbilities[i]->IsShowOnCmd() == false)continue;
		AbilityList[i]->RefreshCell(UnitAbilities[i]->GetAbilityName(),UnitAbilities[i]->GetCost());
		AbilityList[i]->SetVisibility(ESlateVisibility::Visible);
		AbilityNum++;
	}
	i--;
	for(;i < AbilityList.Num();i++)
	{
		AbilityList[i]->SetVisibility(ESlateVisibility::Hidden);
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
