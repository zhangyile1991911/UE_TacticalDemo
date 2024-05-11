// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_AddUnit.h"

#include "Grid.h"
#include "MyCombatSystem.h"
#include "My_Pawn.h"

void AAction_AddUnit::ExecuteAction(const FIntPoint& index)
{
	Super::ExecuteAction(index);

	bool isValid = MyPlayerPawn->GetMyGrid()->IsValidGridIndex(index);
	if(!isValid)return;
	
	bool hasUnit = MyPlayerPawn->GetMyGrid()->TileGridHasUnit(index);
	if(hasUnit)return;

	MyPlayerPawn->GetMyCombatSystem()->AddUnitInCombat(index,MyPlayerPawn->GetCurrentSelectedUnitType());
}
