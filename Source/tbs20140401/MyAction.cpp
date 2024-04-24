// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAction.h"

#include "Action_SelectTile.h"
#include "My_Pawn.h"
#include "Kismet/GameplayStatics.h"

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
	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(),AMy_Pawn::StaticClass());
	MyPlayerPawn = Cast<AMy_Pawn>(actor);
}

// Called every frame
void AMyAction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

