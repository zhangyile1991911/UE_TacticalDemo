// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_FindPath.h"

#include "Grid.h"
#include "MyGridPathfinding.h"
#include "MyUnit.h"
#include "My_Pawn.h"
#include "Kismet/GameplayStatics.h"

void AAction_FindPath::BeginPlay()
{
	Super::BeginPlay();
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(),AMyGridPathfinding::StaticClass());
	MyGridPathfinding = Cast<AMyGridPathfinding>(actor);
}

void AAction_FindPath::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	for(const FIntPoint& one : Path)
    {
    	MyPlayerPawn->GetMyGrid()->RemoveStateFromTile(one,ETileState::PathFinding);
    }
}

void AAction_FindPath::ExecuteAction(const FIntPoint& index)
{
	Super::ExecuteAction(index);
	Start = MyPlayerPawn->GetSelectedTile();
	Finish = index;

	for(const FIntPoint& one : Path)
	{
		MyPlayerPawn->GetMyGrid()->RemoveStateFromTile(one,ETileState::PathFinding);
	}
	
	MyGridPathfinding->FindPath(Start,Finish,FPathFindingCompleted::CreateUObject(this,&AAction_FindPath::WaitPathFinding));
	
}

void AAction_FindPath::WaitPathFinding(TArray<FIntPoint> path)
{
	Path = MoveTemp(path);
	for(const FIntPoint& one : Path)
    {
    	MyPlayerPawn->GetMyGrid()->AddStateToTile(one,ETileState::PathFinding);	
    }
	MyPlayerPawn->GetSelectedUnit()->SetWalkPath(MoveTemp(Path));
}
