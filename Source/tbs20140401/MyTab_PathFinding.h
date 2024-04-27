// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyTab_PathFinding.generated.h"

class AMyGridPathfinding;
class AMy_Pawn;
class UMyButtonAction;
class UCheckBox;
class AMyDebugTextAndColorsOnTiles;
/**
 * 
 */
UCLASS()
class TBS20140401_API UMyTab_PathFinding : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCheckBox> CheckShowIndexes;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCheckBox> CheckIncludeDiagonals;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UMyButtonAction> ShowTileNeighborsBtn;
	
	UPROPERTY()
	AMyDebugTextAndColorsOnTiles* MyDebugTextAndColorsOnTiles;

	bool ShowTilesIndexes = false;
	bool IncludeDiagonals = false;

	UPROPERTY()
	TObjectPtr<AMy_Pawn> My_Pawn;

	UPROPERTY()
	AMyGridPathfinding* MyPathFinding;
public:
	virtual bool Initialize() override;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	TObjectPtr<AMy_Pawn> GetMyPawn();
	AMyGridPathfinding* GetMyPathFinding();
	
	UFUNCTION()
	void OnCheckIndexesChanged(bool isChecked);
	UFUNCTION()
	void OnCheckDiagonals(bool isChecked);

	UFUNCTION()
	void ShowTileNeighborsClicked();
};
