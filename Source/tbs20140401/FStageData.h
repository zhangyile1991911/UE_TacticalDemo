#pragma once

#include "CoreMinimal.h"
#include "Dialogue.h"
#include "UnitData.h"
#include "FStageData.generated.h"

USTRUCT(BlueprintType)
struct FStageData:public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText StageDataPath;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText StageLevelName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSoftObjectPtr<UDialogue> ResOfDialogue;
	//行動優先度補正
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TMap<ETBSUnitType,int> UnitPriorityCorrection;

	//スキルポイント補正
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TMap<ETBSUnitType,int> UnitAPCorrection;
};
