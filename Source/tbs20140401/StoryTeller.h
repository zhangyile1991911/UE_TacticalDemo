// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StoryTeller.generated.h"

class ADialogueController;
DECLARE_DELEGATE(FStoryCompleted);

UCLASS(Blueprintable)
class TBS20140401_API AStoryTeller : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStoryTeller();

protected:
	UPROPERTY()
	TObjectPtr<ADialogueController> DialogueController;
	UFUNCTION()
	void ListenDialogueEnded();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartStory();
	
	void ToNext();
	
	FStoryCompleted StoryCompleted;
};
