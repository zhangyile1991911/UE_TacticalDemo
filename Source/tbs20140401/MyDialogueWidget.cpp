// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDialogueWidget.h"

#include "MyCombatSystem.h"
#include "My_Pawn.h"

AMyUnit* UMyDialogueWidget::GetSpeakerUnit()
{
	if(MyPawn == nullptr)
	{
		const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		APawn* Pawn = PlayerController->GetPawnOrSpectator();
		// APawn* Pawn = PlayerController->GetPawn();
		MyPawn = Cast<AMy_Pawn>(Pawn);
		// return MyPawn->GetMyCombatSystem()->GetFirstUnit();
	}
	return MyPawn->GetMyCombatSystem()->GetFirstUnit();
}
