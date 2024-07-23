#pragma once

#include "CoreMinimal.h"
#include "SkillData.generated.h"

class AUnitAbilityAnim;

//todo 之后将技能改名为Ability
enum class ESkillType
{
	
};
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

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int AllowableDeviation = 1;//允许高度差

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<AUnitAbilityAnim> SkillAnim;//技能演出

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText Description;//技能描述

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icon;
};
