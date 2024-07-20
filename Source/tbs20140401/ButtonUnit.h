// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnitData.h"
#include "ButtonUnit.generated.h"


class UMyGameInstance;
class UBorder;
class UImage;
class UButton;
/**
 * 
 */
UCLASS(Abstract)
class TBS20140401_API UButtonUnit : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> UnitButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> UnitImage;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBorder> Border_IsSelected;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ETBSUnitType UnitType = ETBSUnitType::Warrior;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsLookingRight=false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsSelected = false;

	UPROPERTY()
	UMyGameInstance* GameInstance;
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	void RefreshButtonStyle();
	UFUNCTION()
	void OnUnitButtonClicked();
public:
	DECLARE_DYNAMIC_DELEGATE_OneParam(FUButtonUnitClick,UButtonUnit*,Param);
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FUButtonUnitClick OnClickTopic;
	void SetIsSelected(bool selected);
	ETBSUnitType GetUnitType()const{return UnitType;}
	bool GetIsSelected()const{return IsSelected;}
	bool GetIsLookingRight()const{return IsLookingRight;}
};

