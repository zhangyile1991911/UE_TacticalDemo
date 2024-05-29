#pragma once

#include "CoreMinimal.h"
#include "BattleReport.generated.h"
class AMyUnit;

USTRUCT(BlueprintType)
struct FBattleReport
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AMyUnit* Attacker;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AMyUnit* Defender;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AMyUnit* Cooperator;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsBackAtk;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsHit;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsCritical;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int HitPercent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int RandSeed;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Damage;
	
};
