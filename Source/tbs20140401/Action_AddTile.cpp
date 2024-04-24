// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_AddTile.h"

#include "My_Pawn.h"


// Sets default values
AAction_AddTile::AAction_AddTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AAction_AddTile::ExecuteAction(const FIntPoint& index)
{
	// Super::ExecuteAction(index);
	MyPlayerPawn->AddNewTileUnderCursor();
}

// Called when the game starts or when spawned
void AAction_AddTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAction_AddTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

