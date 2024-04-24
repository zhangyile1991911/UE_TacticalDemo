// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyAction.generated.h"

class AMy_Pawn;
class AMyPlayerActions;

UCLASS(Abstract)
class TBS20140401_API AMyAction : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyAction();

protected:
	UPROPERTY()
	AMy_Pawn* MyPlayerPawn;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void ExecuteAction(const FIntPoint& index){}

};
