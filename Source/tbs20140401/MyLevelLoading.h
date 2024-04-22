// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyLevelLoading.generated.h"

UCLASS()
class TBS20140401_API AMyLevelLoading : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyLevelLoading();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FString curLevel;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void LoadLevel(const FString& loadedLevel);
};
