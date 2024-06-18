// Fill out your copyright notice in the Description page of Project Settings.


#include "BottomActionBar.h"

#include "CmdWidget.h"
#include "FirstRolePortrait.h"
#include "MyCombatSystem.h"
#include "UnitInfoDetail.h"
#include "UnitPortrait.h"
#include "Kismet/GameplayStatics.h"

void UBottomActionBar::NativeConstruct()
{
	Super::NativeConstruct();

	Portraits.Reserve(9);

	Portraits.Add(P1);
	Portraits.Add(P2);
	Portraits.Add(P3);
	Portraits.Add(P4);
	Portraits.Add(P5);
	Portraits.Add(P6);
	Portraits.Add(P7);
	Portraits.Add(P8);
	Portraits.Add(P9);

	AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(),AMyCombatSystem::StaticClass());
	AMyCombatSystem* CombatSystem = Cast<AMyCombatSystem>(Actor);

	CombatSystem->ReSortEvent.AddUObject(this,&UBottomActionBar::OnActionBarChanged);
	CmdList->SetVisibility(ESlateVisibility::Hidden);

	UnitDetailInfoPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void UBottomActionBar::NativeDestruct()
{
	Super::NativeDestruct();
}

void UBottomActionBar::OnActionBarChanged(const TArray<TObjectPtr<AMyUnit>>& array)
{
	if(array.IsEmpty())return;

	auto first = array[0];
	FirstRolePortrait->RefreshByUnit(first);

	
	if(array.Num() - 1 > Portraits.Num())
	{
		for(int i = 1;i < Portraits.Num();i++)
		{
			Portraits[i-1]->RefreshUnitIcon(array[i]);
		}
	}
	else if(array.Num() - 1 < Portraits.Num())
	{
		int PortraitIndex = 0;
		
		for(int i = 1;i < array.Num();i++,PortraitIndex++)
		{
			Portraits[PortraitIndex]->RefreshUnitIcon(array[i]);
		}
		 int count = Portraits.Num() - PortraitIndex;
		
		while(count > 0)
		{
			int remain = count > array.Num() ? array.Num() : count;
			for(int i = 0;i < remain;i++,PortraitIndex++)
			{
				Portraits[PortraitIndex]->RefreshUnitIcon(array[i]);
			}
			count -= remain;
		}
	}
	else
	{
		for(int i = 1;i < array.Num();i++)
		{
			Portraits[i-1]->RefreshUnitIcon(array[i]);
		}
	}

	
}

TObjectPtr<UCmdWidget> UBottomActionBar::ShowCmdPanel(TObjectPtr<AMyUnit> UnitInstance,int CmdIndex) const
{
	CmdList->SetVisibility(ESlateVisibility::Visible);
	CmdList->RefreshUnitCmd(UnitInstance);
	CmdList->SelectCmd(CmdIndex);
	return CmdList;
}
