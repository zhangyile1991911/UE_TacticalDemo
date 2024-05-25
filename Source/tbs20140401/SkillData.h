#pragma once

#include "CoreMinimal.h"
#include "SkillData.generated.h"

USTRUCT(BlueprintType)
struct FSkillData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int SkillId = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText SkillName = FText::FromName(TEXT(""));

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int SpendPoint = 0;//消耗行动点

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FIntPoint Range = FIntPoint(0,0);//范围
};
