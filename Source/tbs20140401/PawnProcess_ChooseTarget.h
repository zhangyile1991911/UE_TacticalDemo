// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnProcess.h"
#include "PawnProcess_ChooseTarget.generated.h"

class UGameUI_BattleInfo;
class AUnitAbilityAnim;
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
	TArray<FIntPoint> ArrayOfAbilityRange;
	TArray<FIntPoint> ArrayOfIndicatorRange;
	TObjectPtr<AUnitAbilityAnim> ChosenAbilityPtr;
	// TObjectPtr<AUnitAbilityAnim> ChosenAbilityAnim;
	UPROPERTY()
	TObjectPtr<UUGameUI_UnitBriefInfo> UnitBriefInfoInstance;
	UPROPERTY()
	TObjectPtr<UGameUI_BattleInfo> BattleInfoInstance;

	bool bIsBackAttack;
	bool bIsWrapAttack;
protected:
	// void AbilityCompleted(TObjectPtr<AUnitAbilityAnim> Ability);
	// void AbilityCompletedEvent(TObjectPtr<AUnitAbilityAnim> Ability);
	void ShowTargetUnitBriefInfo(const FIntPoint& Index);
	void ClearIndicatorRange();
	UFUNCTION()
	void SubscribeCameraActing();
public:
	virtual void EnterProcess(TObjectPtr<AMy_Pawn> Pawn)override;
	virtual void TickProcess()override;
	virtual void HandleDirectionInput(const FVector2D& Input)override;
	virtual void HandleCancelInput()override;
	virtual void HandleConfirmInput()override;
	virtual void ExitProcess()override;

	virtual void HandleLeftInput()override;
	virtual void HandleRightInput()override;
	virtual void HandleZooming(float Val)override;
	virtual void HandleTabInput()override;
};
