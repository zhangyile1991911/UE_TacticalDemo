#pragma once


#include "CoreMinimal.h"
#include "WeaponData.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName WeaponName;
};
