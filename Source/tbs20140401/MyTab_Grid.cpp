// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTab_Grid.h"

#include "AsyncTreeDifferences.h"
#include "IContentBrowserSingleton.h"
#include "MyButtonAction.h"
#include "MySpinBox_WithName.h"
#include "My_Pawn.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "tbs20140401/Grid.h"
#include "tbs20140401/UMySpinBox_WithName_Vector3.h"
#include "tbs20140401/MySpinBox_WithName_Vector2.h"
#include "tbs20140401/MyLevelLoading.h"

TObjectPtr<AGrid> UMyTab_Grid::GetMyGrid()
{
	if(MyGrid == NULL)
	{
		auto world = GetWorld();
		auto actor = UGameplayStatics::GetActorOfClass(world,AGrid::StaticClass());
		MyGrid = Cast<AGrid>(actor);
	}
	return MyGrid;
}

TObjectPtr<AMy_Pawn> UMyTab_Grid::GetMyPawn()
{
	if(My_Pawn == NULL)
	{
		auto world = GetWorld();
		auto actor = UGameplayStatics::GetActorOfClass(world,AMy_Pawn::StaticClass());
		My_Pawn = Cast<AMy_Pawn>(actor);
	}
	return My_Pawn;
}

TObjectPtr<AMyLevelLoading> UMyTab_Grid::GetMyLevelLoading()
{
	if(MyLevelLoading == NULL)
	{
		auto world = GetWorld();
		auto actor = UGameplayStatics::GetActorOfClass(world,AMyLevelLoading::StaticClass());
		MyLevelLoading = Cast<AMyLevelLoading>(actor);
	}
	return MyLevelLoading;
}

void UMyTab_Grid::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp,Log,TEXT("UMyTab_Grid::NativeConstruct()"))
	GridType->OnSelectionChanged.AddDynamic(this,&UMyTab_Grid::OnGridTypeChanged);
	Location->Vector3Topic.AddUObject(this,&UMyTab_Grid::OnLocationChanged);
	TileCount->Vector2Topic.AddUObject(this,&UMyTab_Grid::OnTileCountChanged);
	TileSize->Vector3Topic.AddUObject(this,&UMyTab_Grid::OnTileSizeChanged);
	OffsetFromGround->TopicChanged.AddUObject(this,&UMyTab_Grid::OnOffsetFromGroundChanged);
	CheckBoxBounds->OnCheckStateChanged.AddDynamic(this,&UMyTab_Grid::OnBoundCheckChanged);
	CheckCenter->OnCheckStateChanged.AddDynamic(this,&UMyTab_Grid::OnCenterCheckChanged);
	CheckBottomLeft->OnCheckStateChanged.AddDynamic(this,&UMyTab_Grid::OnBottomLeftCheckChanged);
	CheckUseEnvironment->OnCheckStateChanged.AddDynamic(this,&UMyTab_Grid::OnUseEnvironmentCheckChanged);
	LevelType->OnSelectionChanged.AddDynamic(this,&UMyTab_Grid::OnLevelTypeChanged);

	SelectTileBtn->ButtonActionCB.AddUObject(this,&UMyTab_Grid::OnSelectTileButtonClick);
	AddRemoveBtn->ButtonActionCB.AddUObject(this,&UMyTab_Grid::OnAddRemoveButtonClick);
	IncreaseBtn->ButtonActionCB.AddUObject(this,&UMyTab_Grid::OnIncreaseButtonClick);
	SetTileType->ButtonActionCB.AddUObject(this,&UMyTab_Grid::OnSetTileTypeClick);

	TileType->OnSelectionChanged.AddDynamic(this,&UMyTab_Grid::OnTileTypeChanged);
	
	TileType->SetVisibility(ESlateVisibility::Collapsed);
	// GetMyGrid();
	// if(MyGrid)
	// {
	// 	Location->SetValue(MyGrid->GetGridCenterLocation());
	// 	TileCount->SetValue(MyGrid->GetGridTileCount());
	// 	TileSize->SetValue(MyGrid->GetGridTileSize());
	// }
}

void UMyTab_Grid::NativeDestruct()
{
	Super::NativeDestruct();
	UE_LOG(LogTemp,Log,TEXT("UMyTab_Grid::NativeDestruct()"))
}

void UMyTab_Grid::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UE_LOG(LogTemp,Log,TEXT("UMyTab_Grid::NativeOnInitialized()"))

	GetWorld()->GetTimerManager().SetTimer(MyTimerHandle,this,&UMyTab_Grid::RepeatedDrawDebugLine,0.2f,true);
}

void UMyTab_Grid::NativePreConstruct()
{
	Super::NativePreConstruct();
	UE_LOG(LogTemp,Log,TEXT("UMyTab_Grid::NativePreConstruct()"))
}

void UMyTab_Grid::OnGridTypeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	UE_LOG(LogTemp,Log,TEXT("UMyTab_Grid::OnGridTypeChanged %s"),*SelectedItem);
	GetMyGrid();
	if(MyGrid == nullptr)return;

	if( TEXT("Square") == SelectedItem)
		MyGrid->SpawnGridShape(EGridShape::Square);
	else if(TEXT("Hexagon") == SelectedItem)
		MyGrid->SpawnGridShape(EGridShape::Hexagon);
	else if(TEXT("Triangle") == SelectedItem)
		MyGrid->SpawnGridShape(EGridShape::Triangle);
}

void UMyTab_Grid::OnLevelTypeChanged(FString SelectedItem,ESelectInfo::Type SelectionType)
{
	GetMyLevelLoading()->LoadLevel(SelectedItem);
}

void UMyTab_Grid::OnLocationChanged(const FVector& newVec)
{
	UE_LOG(LogTemp,Log,TEXT("UMyTab_Grid::OnLocationChanged x = %f y = %f z = %f"),newVec.X,newVec.Y,newVec.Z)
	GetMyGrid();
	if(MyGrid == nullptr)return;

	MyGrid->SpawnGridLocation(newVec);
}

void UMyTab_Grid::OnTileCountChanged(const FVector2D& newVec)
{
	UE_LOG(LogTemp,Log,TEXT("UMyTab_Grid::OnLocationChanged x = %f y = %f"),newVec.X,newVec.Y)
	GetMyGrid();
	if(MyGrid == nullptr)return;
	MyGrid->SpawnGridTileCount(newVec);
}

void UMyTab_Grid::OnTileSizeChanged(const FVector& newVec)
{
	UE_LOG(LogTemp,Log,TEXT("UMyTab_Grid::OnLocationChanged x = %f y = %f z = %f"),newVec.X,newVec.Y,newVec.Z)
	GetMyGrid();
	if(MyGrid == nullptr)return;
	
	MyGrid->SpawnGridSize(newVec);
}

void UMyTab_Grid::OnOffsetFromGroundChanged(float val)
{
	GetMyGrid();
	if(MyGrid == nullptr)return;
	
	MyGrid->SetOffsetFromGround(val);
}

void UMyTab_Grid::OnUseEnvironmentCheckChanged(bool bIsChecked)
{
	GetMyGrid();
	if(MyGrid == nullptr)return;
	MyGrid->SetUseEnvironment(bIsChecked);
}

void UMyTab_Grid::OnBoundCheckChanged(bool bIsChecked)
{
}

void UMyTab_Grid::OnCenterCheckChanged(bool bIsChecked)
{
}

void UMyTab_Grid::OnBottomLeftCheckChanged(bool bIsChecked)
{
}

void UMyTab_Grid::RepeatedDrawDebugLine()
{
	// UE_LOG(LogTemp,Log,TEXT("UMyTab_Grid::RepeatedDrawDebugLine()"))
	
	if(CheckCenter->IsChecked())
	{
		auto center = GetMyGrid()->GetGridCenterLocation();
		DrawDebugSphere(GetWorld(),center,30.0f,6,FColor::Cyan,false,1.0f,0,2.0f);
		CenterPos->SetText(center.ToText());
	}
	else
	{
		CenterPos->SetText(FText::FromString("(999,999,999)"));
	}

	if(CheckBottomLeft->IsChecked())
	{
		auto bottomLeft = GetMyGrid()->GetGridBottomLeft();
		DrawDebugSphere(GetWorld(),GetMyGrid()->GetGridBottomLeft(),30.0f,6,FColor::Yellow,false,1.0f,0,2.0f);
		BottomLeftPos->SetText(bottomLeft.ToText());
	}
	else
	{
		BottomLeftPos->SetText(FText::FromString("(999,999,999)"));
	}

	if(CheckBoxBounds->IsChecked())
	{
		FVector center = GetMyGrid()->GetGridCenterLocation();
		FVector bottomLeft = GetMyGrid()->GetGridBottomLeft();
		DrawDebugBox(GetWorld(),center,center-bottomLeft,FColor::Blue,false,1.0f,0,3.0f);
	}

	if(CheckMouseLocation->IsChecked())
	{
		FVector location = GetMyGrid()->GetCursorLocationOnGrid(GetWorld()->GetFirstPlayerController(),true,GetMyPawn()->CanHoverEmptySpace());	
		DrawDebugSphere(GetWorld(),location,20,5,FColor::Yellow);
		MouseLocation->SetText(FText::Format(NSLOCTEXT("","","{0},{1},{2}"),location.X,location.Y,location.Z));
	}

	if(CheckHoveredTile->IsChecked())
	{
		FIntPoint TileIndex = GetMyGrid()->GetTileIndexUnderCursor(GetWorld()->GetFirstPlayerController(),true,GetMyPawn()->CanHoverEmptySpace());
		HoveredTile->SetText(FText::Format(NSLOCTEXT("", "","({0},{1})"), TileIndex.X, TileIndex.Y));
	}
	
}

void UMyTab_Grid::OnSelectTileButtonClick()
{
	AddRemoveBtn->SetUnSelected();
	
	SetTileType->SetUnSelected();
	if(SelectTileBtn->IsSelected())
	{
		GetMyPawn()->SetSelectedActions(SelectTileBtn->GetLeftAction(),SelectTileBtn->GetRightAction());
	}
	else
	{
		GetMyPawn()->SetSelectedActions(nullptr,nullptr);
	}
}

void UMyTab_Grid::OnAddRemoveButtonClick()
{
	
	SelectTileBtn->SetUnSelected();
	SetTileType->SetUnSelected();
	IncreaseBtn->SetUnSelected();
	if(AddRemoveBtn->IsSelected())
	{
		GetMyPawn()->SetSelectedActions(AddRemoveBtn->GetLeftAction(),AddRemoveBtn->GetRightAction());
	}
	else
	{
		GetMyPawn()->SetSelectedActions(nullptr,nullptr);
	}
}

void UMyTab_Grid::OnIncreaseButtonClick()
{
	SelectTileBtn->SetUnSelected();
	SetTileType->SetUnSelected();
	AddRemoveBtn->SetUnSelected();
	if(IncreaseBtn->IsSelected())
	{
		GetMyPawn()->SetSelectedActions(IncreaseBtn->GetLeftAction(),IncreaseBtn->GetRightAction());
	}
	else
	{
		GetMyPawn()->SetSelectedActions(nullptr,nullptr);
	}
	
}


void UMyTab_Grid::OnSetTileTypeClick()
{
	AddRemoveBtn->SetUnSelected();
	SelectTileBtn->SetUnSelected();
	IncreaseBtn->SetUnSelected();
	if(SetTileType->IsSelected())
	{
		GetMyPawn()->SetSelectedActions(SetTileType->GetLeftAction(),SetTileType->GetRightAction());
		TileType->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		GetMyPawn()->SetSelectedActions(nullptr,nullptr);
	}
}

void UMyTab_Grid::OnTileTypeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if(SelectedItem.Equals(TEXT("None")))
	{
		GetMyPawn()->SetCurrentTileType(ETileType::None);
	}
	else if(SelectedItem.Equals(TEXT("Normal")))
	{
		GetMyPawn()->SetCurrentTileType(ETileType::Normal);
	}
	else if(SelectedItem.Equals(TEXT("DoubleCost")))
	{
		GetMyPawn()->SetCurrentTileType(ETileType::DoubleCost);
	}
	else if(SelectedItem.Equals(TEXT("TripleCost")))
	{
		GetMyPawn()->SetCurrentTileType(ETileType::TripleCost);
	}
	else if(SelectedItem.Equals(TEXT("FlyingUnitsOnly")))
	{
		GetMyPawn()->SetCurrentTileType(ETileType::FlyingUnitsOnly);
	}
}
