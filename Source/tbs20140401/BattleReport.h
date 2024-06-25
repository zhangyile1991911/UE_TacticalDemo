#pragma once

#include "CoreMinimal.h"
#include "BattleReport.generated.h"
class AMyUnit;

USTRUCT(BlueprintType)
struct FHitInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AMyUnit* Defender = nullptr;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AMyUnit* Cooperator = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AMyUnit* CooperatorTarget = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsBackAtk = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsHit = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsCritical = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int HitPercent = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int RandSeed = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Damage = 0;
};

USTRUCT(BlueprintType)
struct FBattleReport
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AMyUnit* Attacker = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AMyUnit* Defender = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AMyUnit* Cooperator = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AMyUnit* CooperatorTarget = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsBackAtk = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsHit = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsCritical = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int HitPercent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int RandSeed;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int Damage;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector FirstIndex;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector SecondIndex;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector ThirdIndex;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FHitInfo> HitInfoList;
};

