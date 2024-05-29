#pragma once

#include "CoreMinimal.h"
#include "BattleFunc.generated.h"

class AMyUnit;
class AGrid;

UCLASS(Abstract)
class TBS20140401_API UBattleFunc : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static bool IsBackAttack(AMyUnit* Attacker,AMyUnit* Defender);

	// UFUNCTION(BlueprintCallable)
	// static bool IsWrapAttack(AMyUnit* Attacker,AMyUnit* Defender,AMyUnit* Cooperator);

	UFUNCTION(BlueprintCallable)
	static bool HasWrapAttackUnit(AMyUnit* Attacker,AMyUnit* Defender,AGrid* Grid);

	UFUNCTION(BlueprintCallable)
	static bool IsCritical(AMyUnit* Attacker,AMyUnit* Defender);

	//ここで　TObjectPtrを使わない理由は　blueprintで使えるようにため
	//TObjectPtr使ったら　UFUNCTIONは正しいパラメーターのタイプが知らなくなりました
	UFUNCTION(BlueprintCallable)
	static AMyUnit* GetWrapAttackUnit(AMyUnit* Attacker,AMyUnit* Defender,AGrid* Grid);
	
	UFUNCTION(BlueprintCallable)
	static int CalculateHitRate(AMyUnit* Attacker,AMyUnit* Defender,AGrid* MyGrid,bool HasWrap,bool IsBackAtk);

	// UFUNCTION(BlueprintCallable)
	// static int CalculateDamage(AMyUnit* Attacker,AMyUnit* Defender,AGrid* MyGrid,bool HasWrap,bool IsBackAtk);
};

