// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitData.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "MyUnit.generated.h"


class UUnitAbility;
class AShadowUnit;
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

	UPROPERTY()
	TObjectPtr<UChildActorComponent> MyChildActor;

	UPROPERTY()
	TObjectPtr<AShadowUnit> MyShadowUnit;
	// UPROPERTY()
	// TObjectPtr<USkeletalMeshComponent> ShadowSkeletalMeshComponent;
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

	UPROPERTY()
	TObjectPtr<UCurveFloat> JumpCurve;

	FUnitData MyData;
	//属性相关
	FUnitData_Stats MyStats;
	FUnitProperty MyProperty;
	int CurrentDistanceToAction;
	
	TArray<FIntPoint> WalkPath;
	TArray<FIntPoint> WalkableTiles;
	int WalkPathIndex;

	//技能相关
	UPROPERTY()
	TArray<TObjectPtr<UUnitAbility>> OwnAbilityList;

	// UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsHovered = false;
	// UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsSelected = false;
	
	FRotator FinishRotateAngles;
	FRotator StartRotateAngles;
	float StartHeight;
	float TargetHeight;
	FVector StartPosition;
	FVector TargetPosition;
	
	UPROPERTY()
	TObjectPtr<AGrid> MyGrid;
	
	UPROPERTY()
	TObjectPtr<AMy_Pawn> My_Pawn;

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

	UFUNCTION()
	void HandleJumpAlpha(float Value);
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
	TArray<ETileType> UnitCanWalkTileType(){return MyStats.ValidTileTypes;}
	bool IsInWalkableRange(const FIntPoint& index);
	bool CanDiagonally()const{return MyStats.CanMoveDiagonally;}

	void SetWalkPath(TArray<FIntPoint>);
	void SetWalkableTile(TArray<FIntPoint>);
	const TArray<FIntPoint>& GetWalkableTiles()const{return WalkableTiles;}
	bool IsInWalkableTile(const FIntPoint& point)const{return WalkableTiles.Find(point) != INDEX_NONE;}
	const FUnitProperty& GetProperty()const{return MyProperty;}

	//行动优先级相关
	int DistanceToAction()const{return CurrentDistanceToAction/MyProperty.DistanceToAction;}
	int GetCurrentDistanceToAction()const{return CurrentDistanceToAction;}
	void ResetCurrentDistanceToAction(){CurrentDistanceToAction = 0;}
	void AddCurrentDistanceToAction(){CurrentDistanceToAction += MyProperty.Agility;}
	EUnitType GetUnitType()const{return UnitType;}

	void MoveShadowOnTile(const FVector& location);
	void ShowShadowUnit();
	void HideShadowUnit();

	const TArray<TObjectPtr<UUnitAbility>>& GetOwnAbilityList()const{return OwnAbilityList;}
};
float CalculateRotationAngle(FVector CurrentForward,FVector InitialDirection,FVector TargetDirection);