// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_SelectTile.h"

#include "GridShapeData.h"
#include "My_Pawn.h"


void AAction_SelectTile::ExecuteAction(const FIntPoint& index)
{
	// MyPlayerPawn->RemoveTileStateByIndex(selectedIndex,ETileState::Selected);
	MyPlayerPawn->UpdateTIleByIndex(index,ETileState::Selected);
	selectedIndex = index;
	// MyPlayerPawn->SetSelectedTile(index);
}

void AAction_SelectTile::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp,Log,TEXT("AAction_SelectTile::BeginDestroy()"))

	if(MyPlayerPawn)MyPlayerPawn->RemoveTileStateByIndex(selectedIndex,ETileState::Selected);
}

void AAction_SelectTile::Destroyed()
{
	Super::Destroyed();
	UE_LOG(LogTemp,Log,TEXT("AAction_SelectTile::Destroyed()"))
}




