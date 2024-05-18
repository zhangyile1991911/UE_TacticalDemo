// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyGridMeshInst.generated.h"
enum class ETileState;

UCLASS()
class TBS20140401_API AMyGridMeshInst : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyGridMeshInst();
protected:
	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> InstancedStaticMeshComponent;
	TArray<FIntPoint> InstancedIndexes;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void InitializeGridMeshInst(UStaticMesh* mesh,UMaterialInstance* mi,FVector color,ECollisionEnabled::Type ce);
	void ClearInstance();
	void AddInstance(FTransform& instancedTransform,FIntPoint index,const TSet<ETileState>& states);
	void RemoveInstance(FIntPoint);
	void UpdateInstance(FIntPoint,const TSet<ETileState>& states);
	void UpdateInstanceTransform(const FIntPoint& index,const FTransform& Transform);
	FLinearColor GetColorFromStates(const TSet<ETileState>& states,float& isFilled);
};
