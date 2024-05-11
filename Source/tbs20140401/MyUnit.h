// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitData.h"
#include "GameFramework/Actor.h"
#include "MyUnit.generated.h"

UCLASS()
class TBS20140401_API AMyUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyUnit();
protected:
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> MySkeletalMeshComponent;

	// UPROPERTY()
	// TObjectPtr<UClass> AnimBPClass;
	// UPROPERTY()
	// TObjectPtr<USkeletalMesh> SkeletalMeshAsset;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EUnitType UnitType;

	uint8 testAnimIndex;
	
	FIntPoint GridIndex;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void testloop();
	const FIntPoint& GetGridIndex()const{return GridIndex;}
	void SetGridIndex(const FIntPoint& index){GridIndex = index;}

	void RefreshUnit(EUnitType LUnitType);
	

};
