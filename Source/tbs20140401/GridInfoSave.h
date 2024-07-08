#pragma once

#include "CoreMinimal.h"
#include "GridInfoSave.generated.h"

USTRUCT(BlueprintType)
struct FTileInfoSave
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FIntPoint CellIndex = FIntPoint(0,0);

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FTransform CellTransform;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Height = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int TypeOfUnitOnCell = 0;//当前格子上放置单位类型

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int DirectionOfUnitOnCell = 0;//放置单位类型朝向

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int TypeOfCell = 0;//格子类型
};

USTRUCT(BlueprintType)
struct FGridInfoSave
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector GridScale;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector GridSize;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector GridBottomLeftCornerLocation;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FIntPoint GridTileCount;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector StartLocation;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FTileInfoSave> Tiles;
};