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
	int i = 0;
	for(;i < UnitAbilities.Num();i++)
	{
		AbilityList[i]->RefreshCell(UnitAbilities[i]->GetAbilityName(),UnitAbilities[i]->GetCost());
		AbilityList[i]->SetVisibility(ESlateVisibility::Visible);
	}
	
	for(;i < AbilityList.Num();i++)
	{
		AbilityList[i]->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCmdWidget::SelectCmd(int index)
{
	if(index < 0 || index >= AbilityList.Num())return;
	if(index == SelectedIndex)return;
	
	if(SelectedIndex >= 0 && SelectedIndex < AbilityList.Num())
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
