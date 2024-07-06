// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDialogueWidget.h"

#include "MyCombatSystem.h"
#include "My_Pawn.h"
#include "StoryTeller.h"

AMyUnit* UMyDialogueWidget::GetSpeakerUnit()
{
	if(MyPawn == nullptr)
	{
		auto PlayerController = GetWorld()->GetFirstPlayerController();
		auto Pawn = PlayerController->GetPawn();
		MyPawn = Cast<AMy_Pawn>(Pawn);
	}
	return MyPawn->GetMyCombatSystem()->GetFirstUnit();
}
