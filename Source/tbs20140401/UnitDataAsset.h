#pragma once
#include "CoreMinimal.h"
#include "Engine/SkeletalMesh.h"
#include "UnitDataAsset.generated.h"


USTRUCT(BlueprintType)
struct FUnitDataAsset:public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSoftObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSoftClassPtr<UAnimInstance> AnimBP;
	

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icon;
};
