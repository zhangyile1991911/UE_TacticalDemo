#pragma once
#include "GridShapeData.h"
#include "TileData.generated.h"

USTRUCT(BlueprintType)
struct FTileData:public FTableRowBase
{
	GENERATED_BODY()

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
	TArray<ETileState> States;
};
