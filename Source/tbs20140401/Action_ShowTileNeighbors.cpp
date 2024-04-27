// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_ShowTileNeighbors.h"

#include "GridShapeData.h"
#include "MyGridPathfinding.h"
#include "Kismet/GameplayStatics.h"
#include "tbs20140401/My_Pawn.h"

void AAction_ShowTileNeighbors::BeginPlay()
{
	Super::BeginPlay();
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(),AMyGridPathfinding::StaticClass());
	MyPathFinding = Cast<AMyGridPathfinding>(actor);
	
}

void AAction_ShowTileNeighbors::ExecuteAction(const FIntPoint& index)
{
	Super::ExecuteAction(index);
	if(index == selectedIndex)return;
	
	MyPlayerPawn->RemoveTileStateByIndex(selectedIndex,ETileState::IsNeighbor);
	for(const FIntPoint& one : CurNeighbors)
	{
		MyPlayerPawn->RemoveTileStateByIndex(one,ETileState::IsNeighbor);
	}
	
	MyPlayerPawn->UpdateTIleByIndex(index,ETileState::IsNeighbor);
	selectedIndex = index;
	CurNeighbors = MyPathFinding->GetValidTileNeighbors(index);
	for(const FIntPoint& one : CurNeighbors)
	{
		MyPlayerPawn->UpdateTIleByIndex(one,ETileState::IsNeighbor);
	}
}



void AAction_ShowTileNeighbors::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp,Log,TEXT("AAction_SelectTile::BeginDestroy()"))

	if(MyPlayerPawn == nullptr)return;

	MyPlayerPawn->RemoveTileStateByIndex(selectedIndex,ETileState::IsNeighbor);
	for(const FIntPoint& one : CurNeighbors)
	{
		MyPlayerPawn->RemoveTileStateByIndex(one,ETileState::IsNeighbor);
	}
}

void AAction_ShowTileNeighbors::Destroyed()
{
	Super::Destroyed();
	UE_LOG(LogTemp,Log,TEXT("AAction_SelectTile::Destroyed()"))
}