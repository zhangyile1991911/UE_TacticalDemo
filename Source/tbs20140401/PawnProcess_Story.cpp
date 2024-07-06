// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_Story.h"

#include "BottomActionBar.h"
#include "MyHUD.h"
#include "My_Pawn.h"
#include "StoryTeller.h"

void UPawnProcess_Story::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	Super::EnterProcess(Pawn);
	PawnInstance->GetMyStoryTeller()->StartStory();
	PawnInstance->GetMyStoryTeller()->StoryCompleted.BindLambda([this]()
	{
		PawnInstance->SwitchToNormal();
	});
	UnitBottomActionBar = PawnInstance->GetMyHUD()->GetGameUI();
	UnitBottomActionBar->SetVisibility(ESlateVisibility::Hidden);
}

void UPawnProcess_Story::HandleConfirmInput()
{
	Super::HandleConfirmInput();
	PawnInstance->GetMyStoryTeller()->ToNext();
}

void UPawnProcess_Story::ExitProcess()
{
	Super::ExitProcess();
	PawnInstance->GetMyStoryTeller()->StoryCompleted.Unbind();
}
