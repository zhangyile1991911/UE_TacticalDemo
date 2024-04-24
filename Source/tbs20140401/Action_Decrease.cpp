// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_Decrease.h"

#include "Grid.h"
#include "My_Pawn.h"

// Sets default values
AAction_Decrease::AAction_Decrease()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAction_Decrease::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAction_Decrease::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAction_Decrease::ExecuteAction(const FIntPoint& index)
{
	Super::ExecuteAction(index);
	if(MyPlayerPawn == nullptr)return;

	if(!MyPlayerPawn->GetMyGrid()->IsValidGridIndex(index))return;

	MyPlayerPawn->GetMyGrid()->IncreaseDecreaseTileHeight(index,false);
}

