// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitData.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "UnitRunTimeProperty.h"
#include "MyUnit.generated.h"


class UIMyUnitAnimation;
class IIMyUnitAnimation;
class AIdleDirection;
class UUnitAbility;
class AUnitAbilityAnim;
class AShadowUnit;
class AMy_Pawn;
class AGrid;
class AMyUnit;
DECLARE_DELEGATE(FPathCompleted)


UENUM(BlueprintType)
enum class EUnitDirectType:uint8
{
	RIGHT UMETA(DisplayName="Right"),
	LEFT UMETA(DisplayName="Left"),
	FORWARD UMETA(DisplayName="Forward"),
	BACKWARD UMETA(DisplayName="Backward")
};

UCLASS(Blueprintable)
class TBS20140401_API AMyUnit : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AMyUnit();
protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	USkeletalMeshComponent* MySkeletalMeshComponent;

	UPROPERTY()
	TObjectPtr<UChildActorComponent> MyChildActor;
	UPROPERTY()
	TObjectPtr<AShadowUnit> MyShadowUnit;

	UPROPERTY()
	TObjectPtr<UChildActorComponent> MyDirectionActor;
	TObjectPtr<AIdleDirection> MyDirection;
	
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
	FIntPoint MoveIndex;

	FIntPoint DodgeIndex;

	FTimeline UnitMovement;
	FTimeline DodgeMovement;

	UPROPERTY()
	TObjectPtr<UCurveFloat> LocationCurve;

	UPROPERTY()
	TObjectPtr<UCurveFloat> RotationCurve;

	UPROPERTY()
	TObjectPtr<UCurveFloat> JumpCurve;

	UPROPERTY()
	TObjectPtr<UCurveFloat> DodgeCurve;

	FUnitData MyData;
	//属性相关
	FUnitData_Stats MyStats;
	FUnitProperty MyProperty;
	FUnitRunTimeProperty MyRuntimeProperty;
	int CurrentDistanceToAction;
	
	TArray<FIntPoint> WalkPath;
	TArray<FIntPoint> WalkableTiles;
	int WalkPathIndex;

	//技能相关 开始
	UPROPERTY()
	TArray<TObjectPtr<UChildActorComponent>> OwnAbilityActorComponents;
	UPROPERTY()
	TArray<TObjectPtr<AUnitAbilityAnim>> OwnAbilityAnimList;
	UPROPERTY()
	TArray<TObjectPtr<UUnitAbility>> OwnAbilityList;
	
	int ChosenAbilityIndex = 0;
	//技能相关 结束
	// UClass* bpAnim;
	// IIMyUnitAnimation* MyUnitAnimation;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UAnimInstance* MyAnimInstance;

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
	FIntPoint TempDestinationGridIndex;
	FIntPoint AbilityTargetGridIndex;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	EUnitDirectType TempIdleDirection;
	
	UPROPERTY()
	TObjectPtr<AGrid> MyGrid;
	
	UPROPERTY()
	TObjectPtr<AMy_Pawn> My_Pawn;

	int WalkNum;
	int AtkNum;
	

	FPathCompleted PathCompleted;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateHoveredAndSelected();
	
	UFUNCTION()
	void OnAnimInstanceCompleted();
	
	UFUNCTION()
	void HandleLocationAlpha(float Value);
	UFUNCTION()
	void FinishLocationAlpha();

	UFUNCTION()
	void HandleDodgeAlpha(float Value);
	UFUNCTION()
	void FinishDodgeAlpha();

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

	void SetWalkPath(TArray<FIntPoint>,FPathCompleted);
	void SaveWalkPath(TArray<FIntPoint>);
	void StartWalkPath(FPathCompleted);
	void SetWalkableTile(TArray<FIntPoint>);
	const TArray<FIntPoint>& GetWalkableTiles()const{return WalkableTiles;}
	bool IsInWalkableTile(const FIntPoint& point)const{return WalkableTiles.Find(point) != INDEX_NONE;}
	const FUnitProperty& GetProperty()const{return MyProperty;}
	
	UFUNCTION(BlueprintCallable)
	const FUnitRunTimeProperty& GetRuntimeProperty()const{return MyRuntimeProperty;}
	int GetUnitSide()const{return MyRuntimeProperty.UnitSide;}
	bool IsSameUnitSide(int UnitSide)const{return MyRuntimeProperty.UnitSide == UnitSide;}

	//行动优先级相关
	int DistanceToAction()const{return CurrentDistanceToAction/MyProperty.DistanceToAction;}
	int GetCurrentDistanceToAction()const{return CurrentDistanceToAction;}
	void ResetCurrentDistanceToAction(){CurrentDistanceToAction = 0;}
	void AddCurrentDistanceToAction(){CurrentDistanceToAction += MyProperty.Agility;}
	EUnitType GetUnitType()const{return UnitType;}

	void MoveShadowOnTile(const FVector& location);
	void ShowShadowUnit();
	void HideShadowUnit();

	void SetTempDestinationGridIndex(FIntPoint location){TempDestinationGridIndex = location;}
	FIntPoint GetTempDestinationGridIndex()const{return TempDestinationGridIndex;}
	bool NeedToMove()const{return TempDestinationGridIndex != GridIndex;}

	void SetAbilityTargetGridIndex(FIntPoint location){AbilityTargetGridIndex = location;}
	const FIntPoint& GetAbilityTargetGridIndex()const{return AbilityTargetGridIndex;}
	

	const TArray<TObjectPtr<UUnitAbility>>& GetOwnAbilityList()const{return OwnAbilityList;}
	void SetChosenAbility(int ChosenIndex);
	TObjectPtr<UUnitAbility> GetChosenAbility(){return OwnAbilityList[ChosenAbilityIndex];}
	TObjectPtr<AUnitAbilityAnim> GetChosenAbilityAnim(){return OwnAbilityAnimList[ChosenAbilityIndex];}
	

	TObjectPtr<AIdleDirection> GetMyDirection()const{return MyDirection;}
	void SetTempIdleDirection(EUnitDirectType Direction){TempIdleDirection = Direction;}
	UFUNCTION(BlueprintCallable)
	void TurnLeft();
	UFUNCTION(BlueprintCallable)
	void TurnRight();
	UFUNCTION(BlueprintCallable)
	void TurnForward();
	UFUNCTION(BlueprintCallable)
	void TurnBack();

	void TurnShadowLeft();
	void TurnShadowRight();
	void TurnShadowForward();
	void TurnShadowBack();
	
	void ShowDirectionArrow();
	void HideDirectionArrow();
	bool CanWalk()const{return WalkNum > 0;}
	void WalkDone(){WalkNum-=1;}
	bool CanAttack()const{return AtkNum > 0;}
	void AttackDone(){AtkNum-=1;}
	//タン始まる前に　計算やプロパティリなどセットする
	void BeforeStartTurn();
	void FinishTurn();

	void RotateSelfByDestination(const FIntPoint& StandIndex,const FIntPoint& TargetIndex);

	void DoDodgeAnim(const FIntPoint& FromIndex);
};
float CalculateRotationAngle(FVector CurrentForward,FVector InitialDirection,FVector TargetDirection);