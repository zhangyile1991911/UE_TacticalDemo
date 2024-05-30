// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitAbilityAnim.generated.h"

class AUnitAbilityAnim;
class AMy_Pawn;
DECLARE_EVENT_OneParam(AUnitAbilityAnim,EAbilityComplete,TObjectPtr<AUnitAbilityAnim>)
DECLARE_DELEGATE_OneParam(FAbilityComplete,TObjectPtr<AUnitAbilityAnim>)
UCLASS(Abstract)
class TBS20140401_API AUnitAbilityAnim : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUnitAbilityAnim();

protected:
	// UFUNCTION(BlueprintCallable)
	// void OnAbilityCalculationCompleted(const TArray<FBattleReport>& Reports);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnAbilityCompleted();
public:
	EAbilityComplete CompletedEvent;
	FAbilityComplete CompletedCallback;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//blueprintで実現する 
	UFUNCTION(BlueprintImplementableEvent)
	void DoAnimation(const FBattleReport& Report,AMy_Pawn* MyPawn);
};
