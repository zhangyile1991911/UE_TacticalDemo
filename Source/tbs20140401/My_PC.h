// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Command.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "My_PC.generated.h"

/**
 * 
 */

class AMy_Pawn;
class AMyUnit;

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

	int CommandIndex;
	TArray<FCommand> Commands;
	
	// UPROPERTY()
	// TObjectPtr<AMyUnit> CurrentControlUnit;
	//
	// UPROPERTY()
	// TObjectPtr<AMy_Pawn> MyPawnInstance;
	
protected:
	void PushCommand(FCommand cmd);
	FCommand PopCommand();
	void ClearCommand();
	
	// void SetCurrentControlUnit(TObjectPtr<AMyUnit> Unit);
public:
	void StartUnitTurn(TObjectPtr<AMyUnit> Unit);
	void FinishUnitTurn();
	TObjectPtr<AMyUnit> GetCurrentControlUnit()const;
	// TObjectPtr<AMy_Pawn> GetMyPawn()const{return MyPawnInstance;}
	
};



