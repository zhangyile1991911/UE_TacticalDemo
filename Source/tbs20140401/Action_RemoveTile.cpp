// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_RemoveTile.h"

#include "My_Pawn.h"


// Sets default values
AAction_RemoveTile::AAction_RemoveTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AAction_RemoveTile::ExecuteAction(const FIntPoint& index)
{
	Super::ExecuteAction(index);
	if(MyPlayerPawn)MyPlayerPawn->RemoveTileUnderCursor(index);
}

// Called when the game starts or when spawned
void AAction_RemoveTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAction_RemoveTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

