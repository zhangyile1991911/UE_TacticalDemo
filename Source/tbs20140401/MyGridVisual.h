// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyGridVisual.generated.h"

struct FTileData;
class AMyGridMeshInst;
class AGrid;

UCLASS()
class TBS20140401_API AMyGridVisual : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyGridVisual();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UPROPERTY()
	TObjectPtr<AGrid> myGrid;
	
	UPROPERTY()
	TObjectPtr<AMyGridMeshInst> myGridMeshInst;
	
	UPROPERTY(meta=(AllowPrivateAccess="true"))
	TObjectPtr<UChildActorComponent> ChildActor_GridInstMesh;

	float OffsetFromGround = 5.0f;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void InitializedGridVisual(AGrid* grid);
	// void SetOffsetFromGround(float offset);
	void DestroyGridVisual();
	void UpdateTileVisual(FTileData);
	void AddTileVisual(FTileData);
	void SetOffsetFromGround(float offset);
};


