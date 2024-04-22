// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridModifier.generated.h"

class UBoxComponent;
enum class ETileType;
enum class EGridShape;

UCLASS()
class TBS20140401_API AGridModifier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridModifier();
protected:
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	UPROPERTY()
	TObjectPtr<UDataTable> LoadedDataTable;
	// UPROPERTY(EditAnywhere,BlueprintReadOnly)
	// TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EGridShape	Shape;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ETileType	Type;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool UseForTileHeight;
public:
	virtual void OnConstruction(const FTransform& Transform) override;
	EGridShape GetShape(){return Shape;}
	ETileType GetTileType(){return Type;}
	bool GetUseForTileHeight(){return UseForTileHeight;}
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
