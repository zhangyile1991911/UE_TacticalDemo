// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyNewDialogueWidget.generated.h"

class AMy_Pawn;
class AMyUnit;
/**
 * 
 */
UCLASS(Blueprintable)
class TBS20140401_API UMyNewDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
	UPROPERTY()
	AMy_Pawn* MyPawn;

	UFUNCTION(BlueprintCallable)
	AMyUnit* GetSpeakerUnit();
};
