// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTab_PathFinding.h"

#include "UnitData.h"
#include "MyButtonAction.h"
#include "MyButtonList_Units.h"
#include "MyDebugTextAndColorsOnTiles.h"
#include "MyGridPathfinding.h"
#include "MySpinBox_WithName.h"
#include "My_Pawn.h"
#include "Components/CheckBox.h"
#include "Kismet/GameplayStatics.h"

bool UMyTab_PathFinding::Initialize()
{
	return Super::Initialize();
	
}

TObjectPtr<AMy_Pawn> UMyTab_PathFinding::GetMyPawn()
{
	if(My_Pawn == NULL)
	{
		auto world = GetWorld();
		auto actor = UGameplayStatics::GetActorOfClass(world,AMy_Pawn::StaticClass());
		My_Pawn = Cast<AMy_Pawn>(actor);
	}
	return My_Pawn;
}

AMyGridPathfinding* UMyTab_PathFinding::GetMyPathFinding()
{
	if(MyPathFinding == NULL)
	{
		auto world = GetWorld();
		auto actor = UGameplayStatics::GetActorOfClass(world,AMyGridPathfinding::StaticClass());
		MyPathFinding = Cast<AMyGridPathfinding>(actor);
	}
	return MyPathFinding;
}

void UMyTab_PathFinding::NativeConstruct()
{
	Super::NativeConstruct();

	CheckShowIndexes->OnCheckStateChanged.AddDynamic(this,&UMyTab_PathFinding::OnCheckIndexesChanged);
	CheckShowCost->OnCheckStateChanged.AddDynamic(this,&UMyTab_PathFinding::OnCheckIndexesChanged);
	CheckShowFromStart->OnCheckStateChanged.AddDynamic(this,&UMyTab_PathFinding::OnCheckIndexesChanged);
	CheckShowToTarget->OnCheckStateChanged.AddDynamic(this,&UMyTab_PathFinding::OnCheckIndexesChanged);
	
	CheckIncludeDiagonals->OnCheckStateChanged.AddDynamic(this,&UMyTab_PathFinding::OnCheckDiagonals);
	CheckCanFly->OnCheckStateChanged.AddDynamic(this,&UMyTab_PathFinding::OnCheckCanFly);
	
	
	
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(),AMyDebugTextAndColorsOnTiles::StaticClass());
	MyDebugTextAndColorsOnTiles = Cast<AMyDebugTextAndColorsOnTiles>(actor);

	MaxCalculation->TopicChanged.AddUObject(this,&UMyTab_PathFinding::OnCalculationChanged);

	CheckShowTileType->OnCheckStateChanged.AddDynamic(this,&UMyTab_PathFinding::OnCheckTileTypeChanged);
	
	ButtonList_Units->OnUnitTypeChanged.BindDynamic(this,&UMyTab_PathFinding::OnUnitTypeChanged);

	SelectAndFindBtn->ButtonActionCB.AddUObject(this,&UMyTab_PathFinding::OnSelectAndFindClick);
	ShowTileNeighborsBtn->ButtonActionCB.AddUObject(this,&UMyTab_PathFinding::ShowTileNeighborsClicked);
	AddRemoveUnitBtn->ButtonActionCB.AddUObject(this,&UMyTab_PathFinding::AddRemoveUnitClicked);
	RotateUnitBtn->ButtonActionCB.AddUObject(this,&UMyTab_PathFinding::RotateUnitClicked);
	
}

void UMyTab_PathFinding::NativeDestruct()
{
	Super::NativeDestruct();
	
}

void UMyTab_PathFinding::OnCheckTileTypeChanged(bool isChecked)
{
	if(isChecked)
	{
		MyDebugTextAndColorsOnTiles->ShowAllTileType();
	}
	else
	{
		MyDebugTextAndColorsOnTiles->ClearAllDebugInfo();
	}
}

void UMyTab_PathFinding::OnCheckIndexesChanged(bool isChecked)
{
	ShowTilesIndexes = isChecked;
	MyDebugTextAndColorsOnTiles->SetShowTileText(ShowTilesIndexes);

	GetMyPathFinding()->SetShowDebugOption(CheckShowCost->IsChecked(),
		CheckShowFromStart->IsChecked(),
		CheckShowToTarget->IsChecked());
}

void UMyTab_PathFinding::OnCheckDiagonals(bool isChecked)
{
	GetMyPathFinding()->FindPathSetting(isChecked);
}

void UMyTab_PathFinding::OnCheckCanFly(bool isChecked)
{
	GetMyPathFinding()->CanFly(isChecked);
}

void UMyTab_PathFinding::OnSelectAndFindClick()
{
	ShowTileNeighborsBtn->SetUnSelected();
	AddRemoveUnitBtn->SetUnSelected();
	RotateUnitBtn->SetUnSelected();
	
	if(SelectAndFindBtn->IsSelected())
	{
		GetMyPawn()->SetSelectedActions(SelectAndFindBtn->GetLeftAction(),SelectAndFindBtn->GetRightAction());
	}
	else
	{
		GetMyPawn()->SetSelectedActions(nullptr,nullptr);
	}
}

void UMyTab_PathFinding::ShowTileNeighborsClicked()
{
	SelectAndFindBtn->SetUnSelected();
	AddRemoveUnitBtn->SetUnSelected();
	RotateUnitBtn->SetUnSelected();
	if(ShowTileNeighborsBtn->IsSelected())
	{
		GetMyPathFinding()->FindPathSetting(CheckIncludeDiagonals->IsChecked());
		GetMyPawn()->SetSelectedActions(ShowTileNeighborsBtn->GetLeftAction(),ShowTileNeighborsBtn->GetRightAction());
	}
	else
	{
		GetMyPawn()->SetSelectedActions(nullptr,nullptr);
	}
}

void UMyTab_PathFinding::OnCalculationChanged(float val)
{
	GetMyPathFinding()->SetMaxCalculationPerFrame(val);
}

void UMyTab_PathFinding::AddRemoveUnitClicked()
{
	ShowTileNeighborsBtn->SetUnSelected();
	SelectAndFindBtn->SetUnSelected();
	RotateUnitBtn->SetUnSelected();
	
	if(AddRemoveUnitBtn->IsSelected())
	{
		GetMyPawn()->SetCurrentSelectedUnitType(ButtonList_Units->GetCurrentSelectedType());
		GetMyPawn()->SetSelectedActions(AddRemoveUnitBtn->GetLeftAction(),AddRemoveUnitBtn->GetRightAction());
	}
	else
	{
		GetMyPawn()->SetSelectedActions(nullptr,nullptr);
	}
}

void UMyTab_PathFinding::RotateUnitClicked()
{
	ShowTileNeighborsBtn->SetUnSelected();
	SelectAndFindBtn->SetUnSelected();
	AddRemoveUnitBtn->SetUnSelected();
	
	if(RotateUnitBtn->IsSelected())
	{
		GetMyPawn()->SetSelectedActions(RotateUnitBtn->GetLeftAction(),RotateUnitBtn->GetLeftAction());
	}
	else
	{
		GetMyPawn()->SetSelectedActions(nullptr,nullptr);
	}
}

void UMyTab_PathFinding::OnUnitTypeChanged(ETBSUnitType UnitType)
{
	GetMyPawn()->SetCurrentSelectedUnitType(UnitType);
}
