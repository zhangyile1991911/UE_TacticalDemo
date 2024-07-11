// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_LoadStage.h"

#include "GameSystemPanel.h"
#include "GridDataHelper.h"
#include "GridInfoSave.h"
#include "MyHUD.h"
#include "My_Pawn.h"

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

	FGridInfoSave MyGridData;
	int LevelNum = PawnInstance->GetStageLevelNum();
    const FString Path = FString::Format(TEXT("{0}Content/GridData/Stage{1}.json"),{FPaths::ProjectDir(),LevelNum});
    // const FString LoadPath = FPaths::Combine(FPaths::ProjectDir(), Path);
    const bool bIsSuccess = UGridDataHelper::LoadGridFromJson(MyGridData, Path);
    if(!bIsSuccess)
    {
        UE_LOG(LogTemp,Log,TEXT("%s | %s is failed"),*FPaths::ProjectDir(),*Path)
        return;
    }
    
    bIsLoaded = false;
	const bool bInstantiate = UGridDataHelper::InstantiateGrid(MyGridData,
		PawnInstance->GetMyGrid(),
		PawnInstance->GetMyCombatSystem(),
		PawnInstance);
	
	if(bInstantiate)
	{
		bIsLoaded = true;
	}
	else
	{
		PawnInstance->SwitchToLoadFailed();
	}
	
	
}

void UPawnProcess_LoadStage::TickProcess()
{
	
}

void UPawnProcess_LoadStage::ExitProcess()
{
	
	GameSystemPanel = nullptr;
}
