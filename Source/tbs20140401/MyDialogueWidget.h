// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyDialogueWidget.generated.h"

class AMyUnit;
class AMy_Pawn;
/**
 * 
 */
UCLASS(Blueprintable)
class TBS20140401_API UMyDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	TObjectPtr<AMy_Pawn> MyPawn;

	UFUNCTION(BlueprintCallable)
	AMyUnit* GetSpeakerUnit();
	
};
