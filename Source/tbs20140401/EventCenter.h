// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EventCenter.generated.h"

struct FTileData;
/**
 * 
 */
DECLARE_EVENT_OneParam(UEventCenter,SelectedUnit,uint32)
DECLARE_EVENT_OneParam(UEventCenter,SelectedGrid,const FTileData*)
DECLARE_EVENT_OneParam(UEventCenter,ProcessType,FText)

UCLASS()
class TBS20140401_API UEventCenter : public UObject
{
	GENERATED_BODY()

public:
	SelectedUnit EventOfChosenUnit;
	SelectedGrid EventOfChoseGrid;
	ProcessType  EventOfProcessChanged;
};
