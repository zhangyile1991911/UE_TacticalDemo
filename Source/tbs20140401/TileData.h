﻿#pragma once
#include "GridShapeData.h"
#include "TileData.generated.h"

class AMyUnit;

USTRUCT(BlueprintType)
struct FTileData
{
	GENERATED_BODY()
	virtual ~FTileData() = default;
	// FTileData(FTileData&& Other) noexcept :
	// Index(MoveTemp(Other.Index)),
	// TileType(MoveTemp(Other.TileType)),
	// Transform(MoveTemp(Other.Transform))
	// {
	// }
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FIntPoint Index = FIntPoint(0,0);
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ETileType TileType = ETileType::None;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FTransform Transform;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSet<ETileState> States;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<AMyUnit> UnitOnTile;
};
