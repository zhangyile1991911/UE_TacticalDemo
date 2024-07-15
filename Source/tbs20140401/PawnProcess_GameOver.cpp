// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess_GameOver.h"

#include "GameSystemPanel.h"
#include "MyHUD.h"
#include "My_Pawn.h"

void UPawnProcess_GameOver::HandleConfirmInput()
{
	Super::HandleConfirmInput();
}

void UPawnProcess_GameOver::EnterProcess(TObjectPtr<AMy_Pawn> Pawn)
{
	Super::EnterProcess(Pawn);
	PawnInstance->GetMyHUD()->GetGameSystemPanel()->ShowGameOver();
}

void UPawnProcess_GameOver::TickProcess()
{
	Super::TickProcess();
}

void UPawnProcess_GameOver::ExitProcess()
{
	Super::ExitProcess();
}
