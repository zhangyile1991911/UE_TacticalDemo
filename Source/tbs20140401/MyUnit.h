// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitData.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "MyUnit.generated.h"


class AMy_Pawn;
class AGrid;

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
	EUnitType UnitType = EUnitType::Warrior;

	// uint8 testAnimIndex;
	
	FIntPoint GridIndex;

	FTimeline UnitMovement;

	UPROPERTY()
	TObjectPtr<UCurveFloat> LocationCurve;

	UPROPERTY()
	TObjectPtr<UCurveFloat> RotationCurve;

	FUnitData_Stats MyStats;

	TArray<FIntPoint> WalkPath;
	int WalkPathIndex;

	// UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsHovered = false;
	// UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsSelected = false;
	
	FRotator FinishRotateAngles;
	FRotator StartRotateAngles;
	
	UPROPERTY()
	TObjectPtr<AGrid> MyGrid;
	
	UPROPERTY()
	TObjectPtr<AMy_Pawn> My_Pawn;

	int aaaa = 0;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateHoveredAndSelected();
	
	UFUNCTION()
	void HandleLocationAlpha(float Value);
	UFUNCTION()
	void FinishLocationAlpha();

	UFUNCTION()
	void HandleRotationAlpha(float Value);
	// UFUNCTION()
	// void FinishRotationAlpha();
public:
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// void testloop();
	const FIntPoint& GetGridIndex()const{return GridIndex;}
	void SetGridIndex(const FIntPoint& index){GridIndex = index;}

	void RefreshUnit(TObjectPtr<AMy_Pawn> My_Pawn,TObjectPtr<AGrid> grid,const FIntPoint& index);

	void SetHovered(bool h);
	void SetSelected(bool h);

	bool UnitCanWalkOnTile(ETileType TileType);
	bool CanDiagonally()const{return MyStats.CanMoveDiagonally;}

	void SetWalkPath(TArray<FIntPoint>);

};
float CalculateRotationAngle(FVector CurrentForward,FVector InitialDirection,FVector TargetDirection);