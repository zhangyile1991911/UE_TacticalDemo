// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathPointInst.generated.h"

class AGrid;
class UHierarchicalInstancedStaticMeshComponent;
class UInstancedStaticMeshComponent;
UCLASS(BlueprintInternalUseOnlyHierarchical)
class TBS20140401_API APathPointInst : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> PointMesh;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSoftObjectPtr<UMaterialInstance> PointMaterial;
	
	UPROPERTY()
	TObjectPtr<UHierarchicalInstancedStaticMeshComponent> InstancedStaticMeshComponent;
	// UPROPERTY()
	// TObjectPtr<UInstancedStaticMeshComponent> InstancedStaticMeshComponent;
	
	TArray<int> ArrayOfMeshInstPool;

	int UsedIndex;
	float PointGap = 100.0f;
public:
	// Sets default values for this actor's properties
	APathPointInst();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void CreateShowPoint(int Num);
	int TakeMeshInstIndex();
	void RecycleInstIndex();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ShowPathPoint(const TArray<FIntPoint>& Path,TObjectPtr<AGrid> MyGrid);
	void HidePathPoint();
};


