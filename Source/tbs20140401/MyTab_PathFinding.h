// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyTab_PathFinding.generated.h"

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

	UPROPERTY()
	AMyDebugTextAndColorsOnTiles* MyDebugTextAndColorsOnTiles;

	bool ShowTilesIndexes;
public:
	virtual bool Initialize() override;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnCheckIndexesChanged(bool isChecked);
};
