// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_SelectTile.h"

#include "MyPlayerActions.h"

void AAction_SelectTile::ExecuteAction(const FIntPoint& index)
{
	if(MyPlayerActions->GetSelectedTile() == index)
	{
		MyPlayerActions->UpdateTIleByIndex(index,ETileState::Selected);	
	}
	else
	{
		// MyPlayerActions->remove
	}
	
	
	
}
