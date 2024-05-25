// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnProcess.h"

void UPawnProcess::BeginDestroy()
{
	UE_LOG(LogTemp,Log,TEXT("UPawnProcess::BeginDestroy()"))
	UObject::BeginDestroy();
}
