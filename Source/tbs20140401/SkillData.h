#pragma once

#include "CoreMinimal.h"
#include "SkillData.generated.h"

USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int SkillId;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText SkillName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int SpendPoint;//消耗行动点

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FIntPoint Range;//范围
};
