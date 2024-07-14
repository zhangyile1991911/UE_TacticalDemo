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
	MyPawnInstance->StartGame(false);
	
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

	UGridDataHelper::InstantiateGrid(MyGridData,MyGridPtr,MyCombatPtr,MyPawn);

	// for(auto Pair : StageData->UnitPriorityCorrection)
	// {
	// 	auto MyUnit =	MyPawn->GetMyCombatSystem()->GetUnitByType(Pair.Key);	
	// 	if(MyUnit == nullptr)continue;
	// 	MyUnit->SetAgilityCorrection(Pair.Value);
	// }
}
