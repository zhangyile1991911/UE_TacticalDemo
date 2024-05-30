﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnProcess.h"
#include "BattleReport.h"
#include "PawnProcess_CalcAnim.generated.h"

class UUnitAbility;
class AUnitAbilityAnim;
/**
 * 
 */
UCLASS()
class TBS20140401_API UPawnProcess_CalcAnim : public UPawnProcess
{
	GENERATED_BODY()
protected:
	TArray<FIntPoint> AbilityRange;
	TObjectPtr<UUnitAbility> ChosenAbility;
	TObjectPtr<AUnitAbilityAnim> ChosenAbilityAnim;
	FBattleReport Report;
	void AbilityCompleted(TObjectPtr<AUnitAbilityAnim> Ability);
	void AbilityCompletedEvent(TObjectPtr<AUnitAbilityAnim> Ability);
public:
	virtual void EnterProcess(TObjectPtr<AMy_Pawn> Pawn)override;
	virtual void TickProcess()override;
	virtual void HandleDirectionInput(const FVector2D& Input)override;
	virtual void HandleCancelInput()override;
	virtual void HandleConfirmInput()override;
	virtual void ExitProcess()override;
};