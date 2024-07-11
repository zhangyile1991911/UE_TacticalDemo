// Fill out your copyright notice in the Description page of Project Settings.


#include "GridDataHelper.h"

#include "Grid.h"
#include "GridInfoSave.h"
#include "JsonObjectConverter.h"
#include "MyCombatSystem.h"
#include "MyUnit.h"

enum class ETBSUnitType : uint8;

bool UGridDataHelper::SaveGridToJson(const FGridInfoSave& GridData, const FString& FilePath)
{
	FString OutputString;
	FJsonObjectConverter::UStructToJsonObjectString(GridData, OutputString);
	const bool bIsSuccess = FFileHelper::SaveStringToFile(OutputString, *FilePath);
	UE_LOG(LogTemp,Log,TEXT("%s File Save %d"),*FilePath,bIsSuccess)
	return bIsSuccess;
}

bool UGridDataHelper::LoadGridFromJson(FGridInfoSave& GridData, const FString& FilePath)
{
	FString JsonString;
	bool bIsExist = FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath);
	if(!bIsExist)
	{
		UE_LOG(LogTemp,Log,TEXT("%s don't exist"),*FilePath)
		return bIsExist;
	}
	FFileHelper::LoadFileToString(JsonString, *FilePath);
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &GridData, 0, 0);
	return true;
}

bool UGridDataHelper::InstantiateGrid(const FGridInfoSave& MyGridData,AGrid* MyGridPtr, AMyCombatSystem* MyCombatPtr, AMy_Pawn* MyPawn)
{

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

		// FVector Location(
		// 		Tile.CellIndex.X * MyGridData.GridSize.X,
		// 		Tile.CellIndex.Y * MyGridData.GridSize.Y,
		// 		Tile.Height * MyGridData.GridSize.Z
		// 		);
		// Location += MyGridData.StartLocation;
		// FRotator ZeroRot(0,0,0);
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
	
	return true;
	
}


