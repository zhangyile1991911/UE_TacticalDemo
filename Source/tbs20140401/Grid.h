// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridShapeData.h"
#include "MyGridMeshInst.h"
#include "MyGridVisual.h"
#include "tbs20140401/TileData.h"
#include "Grid.generated.h"

class AMyGridVisual;
class UDataTable;
class UInstancedStaticMeshComponent;
class AMyGridMeshInst;

UCLASS()
class TBS20140401_API AGrid : public AActor
{
	GENERATED_BODY()
protected:
	// UPROPERTY(EditAnywhere,BlueprintReadWrite)
	// TObjectPtr<UInstancedStaticMeshComponent> InstanceStaticMesh;
	
	UPROPERTY(meta=(AllowPrivateAccess="true"))
	TObjectPtr<UChildActorComponent> ChildActor_GridVisual;

	UPROPERTY()
	TObjectPtr<AMyGridVisual> GridVisual;
	
	// UPROPERTY(EditAnywhere,BlueprintReadWrite)
	// TObjectPtr<UDataTable> LoadedDataTable;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector GridCenterLocation;

	FVector GridBottomLeftCornerLocation;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector GridTileSize;

	FVector CurGridTileSize;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FIntPoint GridTileCount;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EGridShape GridShape;

	// FGridShapeData* curGridShape = nullptr;
	// float total;
	TMap<FIntPoint,FTileData> GridTiles;

	float OffsetFromGround = 5.0f;
	bool UseEnvironment;
public:	
	// Sets default values for this actor's properties
	AGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	FQuat GetTileRotationFromGridIndex(FIntPoint GridIndex);
	
	ETileType TraceForGround(FTransform& location);
	void SpawnGrid();
public:
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	EGridShape GetGridShapeType()const{return GridShape;}
	void SpawnGridShape(EGridShape GridShape);
	void SpawnGridTileCount(FVector2D tileCount);
	void SpawnGridLocation(FVector location);
	void SpawnGridSize(FVector size);
	FVector GetGridTileSize()const{return GridTileSize;}
	FVector GetGridCenterLocation()const{return GridCenterLocation;}
	FVector GetGridBottomLeft()const{return GridBottomLeftCornerLocation;}
	FVector2D GetGridTileCount()const{return GridTileCount;}
	FGridShapeData* GetGridShape();
	void AddGridTile(const FTileData& data);
	void DestroyGrid();
	FVector GetCursorLocationOnGrid(TObjectPtr<APlayerController> playerIndex,bool traceForGround,bool traceForEmptySpace);
	FIntPoint GetTileIndexUnderCursor(TObjectPtr<APlayerController> playerIndex,bool traceForGround,bool traceForEmptySpace);
	FIntPoint GetTileIndexFromWorldLocation(FVector location);
	FVector GetTileLocationFromGridIndex(FIntPoint GridIndex);
	
	void SetUseEnvironment(bool bis);
	void SetOffsetFromGround(float offset);
	
	void AddStateToTile(FIntPoint index,ETileState stat);
	void RemoveStateFromTile(FIntPoint index,ETileState stat);
	void RemoveStateAllTile(ETileState stat);

	void AddNewOneTIle(FIntPoint index);
	void RemoveOneTIle(FIntPoint index);

	void SetTileTypeByIndex(FIntPoint index,ETileType tileType);
	bool IsValidGridIndex(const FIntPoint&);
	bool IsTileWalkable(const FIntPoint&);

	void IncreaseDecreaseTileHeight(const FIntPoint& index,bool increase);

	const FTileData* GetTileDataByIndex(const FIntPoint& index);
	TObjectPtr<AMyUnit> GetUnitOnTile(const FIntPoint& index)const
	{
		if(GridTiles.Contains(index))
		{
			return GridTiles[index].UnitOnTile;
		}
		return nullptr;
	}
	void RemoveTileDataUnitByIndex(const FIntPoint& index);
	void AddTileDataUnitByIndex(const FIntPoint& index,TObjectPtr<AMyUnit> Unit);

	const TMap<FIntPoint,FTileData>& GetGridTiles(){return GridTiles;}
	TObjectPtr<AMyGridVisual> GetGridVisual(){return GridVisual;}

	bool TileGridHasUnit(const FIntPoint& index);

	int CalcHighDisparity(FIntPoint a,FIntPoint b);
};


