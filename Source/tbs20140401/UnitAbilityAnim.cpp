﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAbilityAnim.h"


// Sets default values
AUnitAbilityAnim::AUnitAbilityAnim()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AUnitAbilityAnim::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnitAbilityAnim::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUnitAbilityAnim::OnAbilityCompleted()
{
	if(CompletedEvent.IsBound())
	{
		CompletedEvent.Broadcast(this);
	}
	if(CompletedCallback.IsBound())
	{
		CompletedCallback.Execute(this);
	}
}

