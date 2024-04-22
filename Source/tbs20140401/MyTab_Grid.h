// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyTab_Grid.generated.h"

class UMySpinBox_WithName;
class AMyLevelLoading;
class UTextBlock;
class UCheckBox;
class UUMySpinBox_WithName_Vector3;
class UMySpinBox_WithName_Vector2;
class AGrid;
class UComboBoxString;
/**
 * 
 */
UCLASS(Abstract)
class TBS20140401_API UMyTab_Grid : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UComboBoxString> GridType;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UComboBoxString> LevelType;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UUMySpinBox_WithName_Vector3> Location;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UMySpinBox_WithName_Vector2> TileCount;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UUMySpinBox_WithName_Vector3> TileSize;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UMySpinBox_WithName> OffsetFromGround;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCheckBox> CheckUseEnvironment;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCheckBox> CheckBoxBounds;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCheckBox> CheckCenter;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCheckBox> CheckBottomLeft;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UTextBlock> CenterPos;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UTextBlock> BottomLeftPos;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCheckBox> CheckMouseLocation;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UTextBlock> MouseLocation;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCheckBox> CheckHoveredTile;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UTextBlock> HoveredTile;
	
	TObjectPtr<AGrid>	MyGrid;
	TObjectPtr<AMyLevelLoading> MyLevelLoading;
protected:
	TObjectPtr<AGrid> GetMyGrid();
	TObjectPtr<AMyLevelLoading> GetMyLevelLoading();
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	
	UFUNCTION()
	void OnGridTypeChanged( FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION()
	void OnLevelTypeChanged(FString SelectedItem,ESelectInfo::Type SelectionType);
	UFUNCTION()
	void OnLocationChanged(const FVector& newVec);
	UFUNCTION()
	void OnTileCountChanged(const FVector2D& newVec);
	UFUNCTION()
	void OnTileSizeChanged(const FVector& newVec);
	UFUNCTION()
	void OnOffsetFromGroundChanged(float val);

	UFUNCTION()
	void OnBoundCheckChanged(bool bIsChecked);
	
	UFUNCTION()
	void OnCenterCheckChanged(bool bIsChecked);

	UFUNCTION()
	void OnBottomLeftCheckChanged(bool bIsChecked);

	UFUNCTION()
	void OnUseEnvironmentCheckChanged(bool bIsChecked);
	
	UFUNCTION()
	void RepeatedDrawDebugLine();



	FTimerHandle MyTimerHandle;
};
