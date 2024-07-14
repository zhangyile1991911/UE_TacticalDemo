﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_LoadStage.h"

#include "FStageData.h"
#include "GameSystemPanel.h"
#include "GridDataHelper.h"
#include "GridInfoSave.h"
#include "MyCombatSystem.h"
#include "MyHUD.h"
#include "MyUnit.h"
#include "My_Pawn.h"
#include "My_Utilities.h"
#include "Kismet/GameplayStatics.h"

void UPawnProcess_LoadStage::InstantiateGrid()
{
	int LevelNum = PawnInstance->GetStageLevelNum();
	FStageData* StageData = GetStageData(LevelNum);
	const FString Path = FString::Format(TEXT("{0}{1}"),
	{FPaths::ProjectDir(),StageData->StageDataPath.ToString()});
	FGridInfoSave MyGridData;
	const bool bIsSuccess = UGridDataHelper::LoadGridFromJson(MyGridData, Path);
	if(!bIsSuccess)
	{
		UE_LOG(LogTemp,Log,TEXT("%s | %s is failed"),*FPaths::ProjectDir(),*Path)
		return;
	}
	//实例化 网格和单位
	const bool bInstantiate = UGridDataHelper::InstantiateGrid(MyGridData,
		PawnInstance->GetMyGrid(),
		PawnInstance->GetMyCombatSystem(),
		PawnInstance);
	if(!bInstantiate)
	{
		PawnInstance->SwitchToLoadFailed();
	}
	for(auto Pair : StageData->UnitPriorityCorrection)
	{
		auto MyUnit =	PawnInstance->GetMyCombatSystem()->GetUnitByType(Pair.Key);	
		if(MyUnit == nullptr)continue;
		MyUnit->SetAgilityCorrection(Pair.Value);
	}
	
	bIsLoaded = true;
}

void UPawnProcess_LoadStage::OnLevelLoaded()
{
	InstantiateGrid();
	GameSystemPanel->WaitingEnter();
}

void UPawnProcess_LoadStage::HandleConfirmInput()
{
	if(bIsLoaded)
	{
		GameSystemPanel->HideLoading();
		PawnInstance->SwitchToBeforeTurn();
	}
	
}

void UPawnProcess_LoadStage::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	Super::EnterProcess(Pawn);

	GameSystemPanel = PawnInstance->GetMyHUD()->GetGameSystemPanel();
	GameSystemPanel->ShowLoading();
	
	int LevelNum = PawnInstance->GetStageLevelNum();
	
    bIsLoaded = false;
	
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("OnLevelLoaded");
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = __LINE__; // 确保每次调用都有唯一的UUID
	FStageData* Data = GetStageData(LevelNum);
	UGameplayStatics::LoadStreamLevel(GetWorld(),
		FName(Data->StageLevelName.ToString()),
		true,
		false
		,LatentInfo);

	
}

void UPawnProcess_LoadStage::TickProcess()
{
	
}

void UPawnProcess_LoadStage::ExitProcess()
{
	
	GameSystemPanel = nullptr;
}