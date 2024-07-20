// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

class UGameSystemPanel;
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

	UPROPERTY(BlueprintType,BlueprintReadWrite)
	TObjectPtr<UGameSystemPanel> GameSystemPanel;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ShowGameUI(bool IsShow);
	
	UFUNCTION(BlueprintImplementableEvent)
	void ShowBattleInfoUI(bool IsShow);

	//选择信息
	TObjectPtr<UBottomActionBar> GetGameUI()const{return GameUI;}

	//战斗信息 伤害 躲闪 暴击
	TObjectPtr<UGameUI_BattleInfo> GetBattleInfoUI()const{return BattleInfoUI;}

	//系统相关 加载画面
	TObjectPtr<UGameSystemPanel> GetGameSystemPanel()const{return GameSystemPanel;}

	UFUNCTION(BlueprintCallable)
	UGameUI_BattleInfo* GetBattleInfoUIForBP()const{return BattleInfoUI;}
};
