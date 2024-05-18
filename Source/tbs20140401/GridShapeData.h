#pragma once
#include "Materials/MaterialInstance.h"
#include "Engine/StaticMesh.h"
#include "CoreMinimal.h"
#include "GridShapeData.generated.h"

UENUM(BlueprintType)
enum class EGridShape
{
	None UMETA(DisplayName = "None"),
	Square UMETA(DisplayName = "Square"),
	Triangle UMETA(DisplayName = "Triangle"),
	Hexagon UMETA(DisplayName = "Hexagon"),
};

UENUM(BlueprintType)
enum class ETileType
{
	None UMETA(DisplayName="None"),
	Normal UMETA(DisplayName="Normal"),
	Obstacle UMETA(DisplayName="Obstacle"),
	DoubleCost UMETA(DisplayName="DoubleCost"),
	TripleCost UMETA(DisplayName="TripleCost"),
	FlyingUnitsOnly UMETA(DisplayName="FlyingUnitsOnly")
};

UENUM(BlueprintType)
enum class ETileState
{
	None UMETA(DisplayName="None"),
	Hovered UMETA(DisplayName="Hovered"),
	Selected UMETA(DisplayName="Selected"),
	IsNeighbor UMETA(DisplayName="IsNeighbor"),
	PathFinding UMETA(DisplayName="PathFinding"),
	Reachable UMETA(DisplayName="Reachable"),
	Invalid UMETA(DisplayName="Invalid")
};


USTRUCT(BlueprintType)
struct FGridShapeData:public FTableRowBase
{
	
	GENERATED_BODY()
public:
	FGridShapeData():MeshSize(100.0f,100.0f,100.0f)
	{
		
	}
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector MeshSize;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> Mesh;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSoftObjectPtr<UMaterialInstance> MaterialInstance;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> FlatMesh;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSoftObjectPtr<UMaterialInstance> FlatMaterial;
	
	// UPROPERTY(EditAnywhere,BlueprintReadWrite)
	// TSoftObjectPtr<UMaterialInstance> FlatFilledMaterial;
	
};
