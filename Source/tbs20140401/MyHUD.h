// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class TBS20140401_API AMyHUD : public AHUD
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void ShowGameUI(bool IsShow);
};
