// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_Increase.h"

#include "Grid.h"
#include "My_Pawn.h"


// Sets default values
AAction_Increase::AAction_Increase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAction_Increase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAction_Increase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAction_Increase::ExecuteAction(const FIntPoint& index)
{
	Super::ExecuteAction(index);

	if(MyPlayerPawn == nullptr)return;

	if(!MyPlayerPawn->GetMyGrid()->IsValidGridIndex(index))return;

	MyPlayerPawn->GetMyGrid()->IncreaseDecreaseTileHeight(index,true);
}

