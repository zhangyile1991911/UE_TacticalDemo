// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAction.h"
#include "My_PC.h"

void UUnitAction::ExecuteAction(const FIntPoint& index)
{
	if(MyPC == nullptr)
	{
		APlayerController* pc = GetWorld()->GetFirstPlayerController();
		MyPC = Cast<AMy_PC>(pc);	
	}
	
}
