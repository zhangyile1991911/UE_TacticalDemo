// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnitInfoDetail.generated.h"

class UHorizontalBox;
class UOverlay;
class AMyUnit;
class UBorder;
class UProgressBar;
class UTextBlock;
class UImage;
class UUnitInfoDetailPortrait;
/**
 * 
 */
UCLASS()
class TBS20140401_API UUnitInfoDetail : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> LevelTxt;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> NameTxt;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> JobTxt;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> HPTxt;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> HPProgress;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> WeaponImg;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> WeaponName;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> AccessoryImgA;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> AccessoryNameA;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> AccessoryImgB;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> AccessoryNameB;
	//------------------Property---------------------------
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PowerTxt;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PhysicalDefNum;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> MagicNum;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> MagicDefNum;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> LuckyNum;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> HitNum;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SpedNum;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> DodgeNum;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> JumpNum;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> MoveNum;
	
	//----------------Ability-----------------------------
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> SkillCellA;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> SKillImgA;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SKillNameA;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> SkillCellB;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> SKillImgB;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SKillNameB;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> SkillCellC;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> SKillImgC;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SKillNameC;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> SkillCellD;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> SKillImgD;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SKillNameD;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> SkillCellE;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> SKillImgE;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SKillNameE;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> SkillCellF;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> SKillImgF;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SKillNameF;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> SkillCellG;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> SKillImgG;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SKillNameG;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> Esoteric;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> EsotericImg;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> EsotericName;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHorizontalBox> PortraitBox;

	UPROPERTY()
	TArray<TObjectPtr<UTextBlock>> ArrayOfSkillName;
	UPROPERTY()
	TArray<TObjectPtr<UImage>> ArrayOfSkillImg;
	UPROPERTY()
	TObjectPtr<UImage>  HalfPortrait;
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSoftClassPtr<UUnitInfoDetailPortrait> PortraitClassPtr;
	UPROPERTY()
	TArray<TObjectPtr<UUnitInfoDetailPortrait>> PortraitPool;
	UPROPERTY()
	TArray<TObjectPtr<AMyUnit>> ArrayOfUnitTeam;
	void ShowUnitDetailInfo(AMyUnit*);

	int FocusIndex;
public:
	void ShowUnitTeamInfo(TArray<TObjectPtr<AMyUnit>> UnitTeam,TObjectPtr<AMyUnit> FocusUnit);
	void HideUnitTeamInfo();
	void NextUnit();
	void PreviousUnit();
};
