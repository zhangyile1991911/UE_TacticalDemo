// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPathFindingData.h"
#include "GameFramework/Actor.h"
#include "MyDebugTextAndColorsOnTiles.generated.h"

class AMyGridPathfinding;
struct FMyPathFindingData;
struct FTileData;
class AGrid;
class ATextRenderActor;
class UTextRenderComponent;

UCLASS()
class TBS20140401_API AMyDebugTextAndColorsOnTiles : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyDebugTextAndColorsOnTiles();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY()
	TObjectPtr<AGrid> myGrid;

	
	TMap<FIntPoint,ATextRenderActor*> SpawnedTexts;
	bool IsShowIndex;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	ATextRenderActor* GetTextActor(const FIntPoint&);
	void DestroyTextActor(const FIntPoint& index);
	void ClearAllTextActors();
	void UpdateTextOnAllTiles();
	void UpdateStateOnTile(const FIntPoint&);
	void UpdateIndexTextOnTile(const FIntPoint& index,const FTileData& data);
	void SetShowTileText(bool ShowIndex);
	void ShowAllTileType();
	void ClearAllDebugInfo();

	void ShowDebugInfos(TArray<FMyPathFindingData> grids,bool ShowCost,bool ShowStart,bool ShowTarget);
	void ShowDebugInfo(const FMyPathFindingData& FindingData,bool ShowCost,bool ShowStart,bool ShowTarget);
	void ClearDebugInfo(const FMyPathFindingData& FindingData);
};
