#pragma once

#include "CoreMinimal.h"
#include "FunitColorDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FUnitColorDataAsset : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText ColorName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSoftObjectPtr<UMaterialInstance> Slot1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSoftObjectPtr<UMaterialInstance> Slot2;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSoftObjectPtr<UMaterialInstance> Slot3;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSoftObjectPtr<UMaterialInstance> Slot4;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSoftObjectPtr<UMaterialInstance> Slot5;
};
