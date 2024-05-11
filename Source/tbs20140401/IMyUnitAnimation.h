// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IMyUnitAnimation.generated.h"


UENUM(BlueprintType)
enum class EUnitAnimation:uint8
{
	Idle UMETA(DisplayName="Idle"),
	Walk UMETA(DisplayName="Walk"),
	Attack UMETA(DisplayName="Attack"),
	Hit UMETA(DisplayName="Hit"),
	Death UMETA(DisplayName="Death"),
	Respawn UMETA(DisplayName="Respawn") 
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIMyUnitAnimation : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TBS20140401_API IIMyUnitAnimation
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void SetUnitAnimationState(EUnitAnimation NewState);
};
