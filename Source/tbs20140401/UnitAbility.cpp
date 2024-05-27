// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAbility.h"

void UUnitAbility::BeginDestroy()
{
	UE_LOG(LogTemp,Log,TEXT("UUnitAbility::BeginDestroy()"))
	UObject::BeginDestroy();
}

void UUnitAbility::OnAbilityCompleted()
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
