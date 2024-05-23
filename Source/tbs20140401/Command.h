#pragma once

#include "CoreMinimal.h"
#include "Command.generated.h"

class AMyUnit;

UENUM(BlueprintType)
enum class ECommandType
{
	None UMETA(DisplayName = "None"),
	Move UMETA(DisplayName = "Move"),
	Attack UMETA(DisplayName = "Attack"),
};

USTRUCT(BlueprintType)
struct FCommand
{
	GENERATED_BODY()
	
	//移动
	FIntPoint DestPoint;
	FIntPoint StartPoint;
	//攻击
	TObjectPtr<AMyUnit> TargetUnit;
};
