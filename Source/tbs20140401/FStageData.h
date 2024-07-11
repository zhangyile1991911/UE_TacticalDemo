#pragma once

#include "FStageData.generated.h"

USTRUCT(BlueprintType)
struct FStageData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText StageDataPath;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText StageLevelName;
};
