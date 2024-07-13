#pragma once

#include "CoreMinimal.h"
#include "FStageData.generated.h"

USTRUCT(BlueprintType)
struct FStageData:public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText StageDataPath;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText StageLevelName;
};
