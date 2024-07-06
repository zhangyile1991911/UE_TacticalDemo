// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyTab_PathFinding.generated.h"

enum class ETBSUnitType : uint8;
class UMyButtonList_Units;
class UMySpinBox_WithName;
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
	TObjectPtr<UCheckBox> CheckShowCost;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCheckBox> CheckShowFromStart;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCheckBox> CheckShowToTarget;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCheckBox> CheckShowTileType;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCheckBox> CheckCanFly;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UMyButtonAction> ShowTileNeighborsBtn;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UMyButtonAction> SelectAndFindBtn;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UMyButtonAction> AddRemoveUnitBtn;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UMyButtonList_Units> ButtonList_Units;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UMySpinBox_WithName> MaxCalculation;
	
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
	void OnCheckTileTypeChanged(bool isChecked);
	UFUNCTION()
	void OnCheckDiagonals(bool isChecked);
	UFUNCTION()
	void OnCheckCanFly(bool isChecked);

	void OnSelectAndFindClick();
	
	UFUNCTION()
	void ShowTileNeighborsClicked();

	UFUNCTION()
	void OnCalculationChanged(float val);

	UFUNCTION()
	void AddRemoveUnitClicked();

	UFUNCTION()
	void OnUnitTypeChanged(ETBSUnitType UnitType);
};
