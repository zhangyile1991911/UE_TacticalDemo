#pragma once
#include "CoreMinimal.h"
#include "UnitDataAsset.h"
#include "UnitData_Stats.h"
#include "SkillData.h"
#include "UnitData.generated.h"

UENUM(BlueprintType)
enum class EUnitType:uint8
{
	Warrior UMETA(DisplayName="Warrior"),
	Ranger UMETA(DisplayName="Ranger"),
	Slime UMETA(DisplayName="Slime"),
	Priest UMETA(DisplayName="Priest"),
	Chicken UMETA(DisplayName="Chicken"),
	Bat UMETA(DisplayName="Bat"),
	EnemyWarrior UMETA(DisplayName="EnemyWarrior"),
	EnemyRanger UMETA(DisplayName="EnemyRanger"),
	EnemySlime UMETA(DisplayName="EnemySlime"),
	EnemyTank UMETA(DisplayName="EnemyTank"),
	EnemyPriest UMETA(DisplayName="EnemyPriest"),
	EnemyChicken UMETA(DisplayName="EnemyChicken"),
	EnemyBat UMETA(DisplayName="EnemyBat"),
};

USTRUCT(BlueprintType)
struct FUnitData:public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EUnitType Type;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FUnitDataAsset Assets;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FUnitData_Stats Stats;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FUnitProperty Property;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FSkillData> Ability;

	
};
