// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BottomActionBar.generated.h"

class UUGameUI_UnitBriefInfo;
class UCmdWidget;
class AMyUnit;
class UUnitPortrait;
class UFirstRolePortrait;
/**
 * 
 */
UCLASS()
class TBS20140401_API UBottomActionBar : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCmdWidget> CmdList;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UFirstRolePortrait> FirstRolePortrait;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUnitPortrait> P1;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUnitPortrait> P2;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUnitPortrait> P3;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUnitPortrait> P4;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUnitPortrait> P5;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUnitPortrait> P6;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUnitPortrait> P7;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUnitPortrait> P8;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUnitPortrait> P9;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUGameUI_UnitBriefInfo> UnitBriefInfo;

	TArray<TObjectPtr<UUnitPortrait>> Portraits;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	void OnActionBarChanged(const TArray<TObjectPtr<AMyUnit>>& array);
public:
	TObjectPtr<UCmdWidget> GetCmdPanel()const{return CmdList;}
	TObjectPtr<UUGameUI_UnitBriefInfo> GetUnitBriefInfo()const{return UnitBriefInfo;}
};
