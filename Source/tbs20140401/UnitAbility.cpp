// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAbility.h"

void UUnitAbility::BeginDestroy()
{
	UE_LOG(LogTemp,Log,TEXT("UUnitAbility::BeginDestroy()"))
	UObject::BeginDestroy();
}
