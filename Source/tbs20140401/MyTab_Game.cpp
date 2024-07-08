// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTab_Game.h"

#include "Grid.h"
#include "GridDataHelper.h"
#include "GridInfoSave.h"
#include "Components/Button.h"
#include "GameFramework/HUD.h"
#include "MyHUD.h"
#include "My_Pawn.h"
#include "MyCombatSystem.h"
#include "MyUnit.h"
#include "Components/EditableText.h"
#include "Kismet/GameplayStatics.h"


void UMyTab_Game::NativeConstruct()
{
	Super::NativeConstruct();

	StartGameBtn->OnClicked.AddDynamic(this,&UMyTab_Game::OnStartGameClicked);
	SaveGridDataBtn->OnClicked.AddDynamic(this,&UMyTab_Game::OnSaveGridDataClicked);
	LoadGridDataBtn->OnClicked.AddDynamic(this,&UMyTab_Game::OnLoadGirdDataClicked);
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		// 获取当前的 HUD 实例
		AHUD* HUD = PlayerController->GetHUD();
		if (HUD)
		{
			// 尝试将 HUD 转换为你的自定义 HUD 类
			MyHUDInstance = Cast<AMyHUD>(HUD);
		}

		APawn* PAWN = PlayerController->GetPawn();
		if(PAWN)
		{
			MyPawnInstance = Cast<AMy_Pawn>(PAWN);
		}
	}
	
}

void UMyTab_Game::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMyTab_Game::OnStartGameClicked()
{
	MyHUDInstance->ShowGameUI(true);
	MyPawnInstance->SetSelectedActions(nullptr,nullptr);
	MyPawnInstance->GetMyCombatSystem()->ResetAllUnit();
	MyPawnInstance->GetMyGrid()->RemoveStateAllTile(ETileState::Selected);
	MyPawnInstance->StartGame();
	
	// TObjectPtr<AMyUnit> firstUnit = MyPawnInstance->GetMyCombatSystem()->SortActionPriority();
	// auto pc = GetWorld()->GetFirstPlayerController();
	// AMy_PC* myPC = Cast<AMy_PC>(pc);
	// if(myPC)
	// {
	// 	myPC->StartUnitTurn(firstUnit);
	// }
}

void UMyTab_Game::OnSaveGridDataClicked()
{
	FGridInfoSave MyGridData;
	MyGridData.Tiles.Reserve(512);
	
	AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(),AGrid::StaticClass());
	AGrid* MyGridPtr = Cast<AGrid>(Actor);
	if(MyGridPtr == nullptr)
	{
		UE_LOG(LogTemp,Log,TEXT("can't get Grid"))
		return;
	}

	MyGridData.GridSize = MyGridPtr->GetGridTileSize();
	MyGridData.GridTileCount = MyGridPtr->GetGridTileCount();
	MyGridData.StartLocation = MyGridPtr->GetActorLocation();
	MyGridData.GridBottomLeftCornerLocation = MyGridPtr->GetGridBottomLeft();
	MyGridData.GridScale = MyGridData.GridSize / MyGridPtr->GetGridShape()->MeshSize;
	const TMap<FIntPoint,FTileData>&  GridData = MyGridPtr->GetGridTiles();
	for(const auto& Pair : GridData)
	{
		FTileInfoSave one;
		one.CellIndex = Pair.Key;
		one.CellTransform = Pair.Value.Transform;
		one.Height = Pair.Value.Height;
		one.TypeOfCell = static_cast<int>(Pair.Value.TileType);
		if(Pair.Value.UnitOnTile != nullptr)
		{
			one.TypeOfUnitOnCell = static_cast<int>(Pair.Value.UnitOnTile->GetUnitType());	
			one.DirectionOfUnitOnCell = static_cast<int>(Pair.Value.UnitOnTile->GetUnitDirect());
		}
		else
		{
			one.TypeOfUnitOnCell = static_cast<int>(ETBSUnitType::None);
		}
		
		MyGridData.Tiles.Add(one);
	}

	
	const FString SavePath = FPaths::Combine(FPaths::ProjectDir(), SaveFilePath->GetText().ToString());
	UGridDataHelper::SaveGridToJson(MyGridData,SavePath);
}

void UMyTab_Game::OnLoadGirdDataClicked()
{
	FGridInfoSave MyGridData;
	const FString LoadPath = FPaths::Combine(FPaths::ProjectDir(), LoadFilePath->GetText().ToString());
	bool bIsSuccess = UGridDataHelper::LoadGridFromJson(MyGridData, LoadPath);
	if(!bIsSuccess)
	{
		return;
	}

	AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(),AGrid::StaticClass());
	AGrid* MyGridPtr = Cast<AGrid>(Actor);
	
	Actor = UGameplayStatics::GetActorOfClass(GetWorld(),AMyCombatSystem::StaticClass());
	AMyCombatSystem* MyCombatPtr = Cast<AMyCombatSystem>(Actor);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;	
	}
	
	AMy_Pawn* MyPawn = Cast<AMy_Pawn>(PlayerController->GetPawn());
    if(MyPawn)
    {
    	MyPawnInstance = Cast<AMy_Pawn>(MyPawn);
    }

	MyGridPtr->SetGridSize(MyGridData.GridSize);
	MyGridPtr->SetActorLocation(MyGridData.StartLocation);
	MyGridPtr->SetGridTileCount(MyGridData.GridTileCount);
	MyGridPtr->SetGridBottomLeft(MyGridData.GridBottomLeftCornerLocation);
	for (const FTileInfoSave& Tile : MyGridData.Tiles)
	{
		// UE_LOG(LogTemp,Log,TEXT("%d %d"),Tile.CellIndex.X,Tile.CellIndex.Y)
		FTileData NewTileData;
		NewTileData.Index = Tile.CellIndex;
		NewTileData.Height = Tile.Height;
		NewTileData.TileType = static_cast<ETileType>(Tile.TypeOfCell);

		FVector Location(
				Tile.CellIndex.X * MyGridData.GridSize.X,
				Tile.CellIndex.Y * MyGridData.GridSize.Y,
				Tile.Height * MyGridData.GridSize.Z
				);
		Location += MyGridData.StartLocation;
		FRotator ZeroRot(0,0,0);
		NewTileData.Transform = Tile.CellTransform;
		MyGridPtr->AddGridTile(NewTileData);

		ETBSUnitType UT = static_cast<ETBSUnitType>(Tile.TypeOfUnitOnCell);
		if(UT != ETBSUnitType::None)
		{
			EUnitDirectType Dir = static_cast<EUnitDirectType>(Tile.DirectionOfUnitOnCell);
			TObjectPtr<AMyUnit> NewUnit = MyCombatPtr->AddUnitInCombatByType(NewTileData.Index,UT,Dir,MyPawn);	
			NewTileData.UnitOnTile = NewUnit;
		}
	}

	
		
	
}
