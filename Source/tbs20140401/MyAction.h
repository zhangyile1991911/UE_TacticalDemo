// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyAction.generated.h"

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
	AMyPlayerActions* MyPlayerActions;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void ExecuteAction(const FIntPoint& index){}
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
