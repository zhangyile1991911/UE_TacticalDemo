// Fill out your copyright notice in the Description page of Project Settings.


#include "StoryTeller.h"

#include "DialogueController.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AStoryTeller::AStoryTeller()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AStoryTeller::ListenDialogueEnded()
{
	StoryCompleted.Execute();
}

// Called when the game starts or when spawned
void AStoryTeller::BeginPlay()
{
	Super::BeginPlay();
	auto Actor = UGameplayStatics::GetActorOfClass(GetWorld(),ADialogueController::StaticClass());
	DialogueController = Cast<ADialogueController>(Actor);
	DialogueController->OnDialogueEnded.AddDynamic(this,&AStoryTeller::ListenDialogueEnded);
}

// Called every frame
void AStoryTeller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStoryTeller::ToNext()
{
	DialogueController->SelectOption(0);
}



