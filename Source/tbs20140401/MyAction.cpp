// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAction.h"

// Sets default values
AMyAction::AMyAction()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
}

// Called when the game starts or when spawned
void AMyAction::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyAction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

