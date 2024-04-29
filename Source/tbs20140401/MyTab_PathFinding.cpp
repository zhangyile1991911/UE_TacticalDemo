// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTab_PathFinding.h"

#include "MyButtonAction.h"
#include "MyDebugTextAndColorsOnTiles.h"
#include "MyGridPathfinding.h"
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
	
	
	SelectAndFindBtn->ButtonActionCB.AddUObject(this,&UMyTab_PathFinding::OnSelectAndFindClick);
	
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(),AMyDebugTextAndColorsOnTiles::StaticClass());
	MyDebugTextAndColorsOnTiles = Cast<AMyDebugTextAndColorsOnTiles>(actor);

	ShowTileNeighborsBtn->ButtonActionCB.AddUObject(this,&UMyTab_PathFinding::ShowTileNeighborsClicked);
}

void UMyTab_PathFinding::NativeDestruct()
{
	Super::NativeDestruct();
	
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

void UMyTab_PathFinding::OnSelectAndFindClick()
{
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
