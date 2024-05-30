// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnProcess.h"
#include "PawnProcess_ChooseTarget.generated.h"

class AUnitAbilityAnim;
class UUnitAbility;
class UUGameUI_UnitBriefInfo;
/**
 * 
 */
UCLASS()
class TBS20140401_API UPawnProcess_ChooseTarget : public UPawnProcess
{
	GENERATED_BODY()
protected:
	FIntPoint CurrentCursor;
	TArray<FIntPoint> AbilityRange;
	TObjectPtr<UUnitAbility> ChosenAbility;
	// TObjectPtr<AUnitAbilityAnim> ChosenAbilityAnim;
	TObjectPtr<UUGameUI_UnitBriefInfo> UnitBriefInfoInstance;
protected:
	// void AbilityCompleted(TObjectPtr<AUnitAbilityAnim> Ability);
	// void AbilityCompletedEvent(TObjectPtr<AUnitAbilityAnim> Ability);
	void ShowTargetUnitBriefInfo(const FIntPoint& Index);
public:
	virtual void EnterProcess(TObjectPtr<AMy_Pawn> Pawn)override;
	virtual void TickProcess()override;
	virtual void HandleDirectionInput(const FVector2D& Input)override;
	virtual void HandleCancelInput()override;
	virtual void HandleConfirmInput()override;
	virtual void ExitProcess()override;
};
