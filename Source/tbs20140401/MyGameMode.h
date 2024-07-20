// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TBS20140401_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:
	// virtual void BeginPlay() override;
public:
	
	UFUNCTION(BlueprintImplementableEvent)
	bool IsGameDemo();
};
