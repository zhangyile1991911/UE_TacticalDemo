// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShadowUnit.generated.h"

class AMyUnit;

UCLASS()
class TBS20140401_API AShadowUnit : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY()
	TObjectPtr<AMyUnit> MyMaster;
public:
	// Sets default values for this actor's properties
	AShadowUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void RefreshUnit(TObjectPtr<AMyUnit> Master,UClass* AnimBP);
	void TurnLeft();
	void TurnRight();
	void TurnForward();
	void TurnBack();
};
