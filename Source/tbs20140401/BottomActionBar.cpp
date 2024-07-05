// Fill out your copyright notice in the Description page of Project Settings.


#include "BottomActionBar.h"

#include "CmdWidget.h"
#include "EventCenter.h"
#include "FirstRolePortrait.h"
#include "MyCombatSystem.h"
#include "My_Pawn.h"
#include "TileData.h"
#include "UnitInfoDetail.h"
#include "UnitPortrait.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
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

	UnitDetailInfoPanel->SetVisibility(ESlateVisibility::Hidden);
	FocusImage->SetVisibility(ESlateVisibility::Hidden);

	if(GetWorld() != nullptr && GetWorld()->GetFirstPlayerController() != nullptr)
	{
		APawn* CurPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
		if(CurPawn == nullptr)return;
		AMy_Pawn* MyPawn = Cast<AMy_Pawn>(CurPawn);
		EventHandlerGrid = MyPawn->GetEventCenter()->EventOfChoseGrid.AddUObject(this,&UBottomActionBar::OnEventGrid);	
		EventHandlerProcess = MyPawn->GetEventCenter()->EventOfProcessChanged.AddUObject(this,&UBottomActionBar::OnEventProcess);
		EventHandleUnitSelected = MyPawn->GetEventCenter()->EventOfChosenUnit.AddUObject(this,&UBottomActionBar::OnEventUnitSelect);
	}
	
}

void UBottomActionBar::NativeDestruct()
{
	Super::NativeDestruct();

	
}

void UBottomActionBar::BeginDestroy()
{
	Super::BeginDestroy();

	if(GetWorld() != nullptr && GetWorld()->GetFirstPlayerController() != nullptr)
    {
    	APawn* CurPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    	if(CurPawn == nullptr)return;
    	AMy_Pawn* MyPawn = Cast<AMy_Pawn>(CurPawn);
		MyPawn->GetEventCenter()->EventOfChoseGrid.Remove(EventHandlerGrid);
		MyPawn->GetEventCenter()->EventOfChoseGrid.Remove(EventHandleUnitSelected);
		MyPawn->GetEventCenter()->EventOfChoseGrid.Remove(EventHandlerProcess);
    }
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

void UBottomActionBar::OnEventGrid(const FTileData* TileDataPtr)
{
	const int Height = TileDataPtr->Transform.GetLocation().Z/100;
	GridHeight->SetText(FText::Format(NSLOCTEXT("","","{0}"),Height));
	switch (TileDataPtr->TileType)
	{
	case ETileType::Normal:
		GridType->SetText(FText::FromName(TEXT("普通")));
		break;
	case ETileType::DoubleCost:
		GridType->SetText(FText::FromName(TEXT("二倍")));
		break;
	case ETileType::TripleCost:
		GridType->SetText(FText::FromName(TEXT("三倍")));
		break;
	case ETileType::Obstacle:
		GridType->SetText(FText::FromName(TEXT("障害物")));
		break;
	case ETileType::FlyingUnitsOnly:
		GridType->SetText(FText::FromName(TEXT("飛ぶのみ")));
		break;
	}
}

void UBottomActionBar::OnEventProcess(FText ProcessText)
{
	ProcessTxt->SetText(ProcessText);
}

void UBottomActionBar::OnEventUnitSelect(uint32 UniqueID)
{
	for(int i = 0;i < Portraits.Num();i++)
	{
		Portraits[i]->OnFocus(UniqueID);
	}
}

TObjectPtr<UCmdWidget> UBottomActionBar::ShowCmdPanel(TObjectPtr<AMyUnit> UnitInstance,int CmdIndex,bool bShowIdle) const
{
	CmdList->SetVisibility(ESlateVisibility::Visible);
	CmdList->RefreshUnitCmd(UnitInstance,bShowIdle);
	CmdList->SelectCmd(CmdIndex);
	return CmdList;
}

void UBottomActionBar::ShowFocusUnit(FVector Location)
{
	if(PlayerControllerPtr == nullptr)
	{
		PlayerControllerPtr = GetWorld()->GetFirstPlayerController();	
	}

	FVector2D ScreenLocation;
	const bool Result = PlayerControllerPtr->ProjectWorldLocationToScreen(Location,ScreenLocation,true);
	if(Result)
	{
		auto M_Image = FocusImage->GetDynamicMaterial();
		if(M_Image)
		{
			int32 w,h;
			PlayerControllerPtr->GetViewportSize(w,h);
			float x = ScreenLocation.X/w;
			float y = ScreenLocation.Y/h;
			M_Image->SetVectorParameterValue(TEXT("WindowSize"),FLinearColor(w,h,0,0));
			M_Image->SetScalarParameterValue(TEXT("ScreenPosX"),x);
			M_Image->SetScalarParameterValue(TEXT("ScreenPosY"),y);
		}
		FocusImage->SetVisibility(ESlateVisibility::Visible);
	}

}

void UBottomActionBar::HideFocus()
{
	FocusImage->SetVisibility(ESlateVisibility::Hidden);
}

void UBottomActionBar::PlayHideBattleUI()
{
	PlayAnimation(StartBattleHideUI);
}

void UBottomActionBar::PlayShowBattleUI()
{
	PlayAnimation(FinishBattleShowUI);
}
