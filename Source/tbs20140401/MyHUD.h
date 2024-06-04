// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

class UBottomActionBar;
class UGameUI_BattleInfo;
/**
 * 
 */
UCLASS(Blueprintable)
class TBS20140401_API AMyHUD : public AHUD
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintType,BlueprintReadWrite)
	TObjectPtr<UBottomActionBar> GameUI;

	UPROPERTY(BlueprintType,BlueprintReadWrite)
	TObjectPtr<UGameUI_BattleInfo> BattleInfoUI;
public:
	UFUNCTION(BlueprintImplementableEvent)
	void ShowGameUI(bool IsShow);
	
	UFUNCTION(BlueprintImplementableEvent)
	void ShowBattleInfoUI(bool IsShow);

	TObjectPtr<UBottomActionBar> GetGameUI()const{return GameUI;}

	TObjectPtr<UGameUI_BattleInfo> GetBattleInfoUI()const{return BattleInfoUI;}

	UFUNCTION(BlueprintCallable)
	UGameUI_BattleInfo* GetBattleInfoUIForBP()const{return BattleInfoUI;}
};
