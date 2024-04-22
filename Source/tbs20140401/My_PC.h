// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "My_PC.generated.h"

/**
 * 
 */


class UInputMappingContext;

UCLASS()
class TBS20140401_API AMy_PC : public APlayerController
{
	GENERATED_BODY()
	virtual void BeginPlay() override;
	
protected:
	AMy_PC();
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

protected:
	
};



