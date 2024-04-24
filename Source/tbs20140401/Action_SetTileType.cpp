// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_SetTileType.h"

#include "My_Pawn.h"


// Sets default values
AAction_SetTileType::AAction_SetTileType()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AAction_SetTileType::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAction_SetTileType::ExecuteAction(const FIntPoint& index)
{
	Super::ExecuteAction(index);
	if(MyPlayerPawn)MyPlayerPawn->UpdateTileTypeUnderCursor(index);
}

// Called every frame
void AAction_SetTileType::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

