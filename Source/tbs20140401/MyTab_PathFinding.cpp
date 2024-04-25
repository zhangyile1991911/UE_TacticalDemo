// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTab_PathFinding.h"

#include "MyDebugTextAndColorsOnTiles.h"
#include "Components/CheckBox.h"
#include "Kismet/GameplayStatics.h"

bool UMyTab_PathFinding::Initialize()
{
	return Super::Initialize();
	
}

void UMyTab_PathFinding::NativeConstruct()
{
	Super::NativeConstruct();

	CheckShowIndexes->OnCheckStateChanged.AddDynamic(this,&UMyTab_PathFinding::OnCheckIndexesChanged);

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(),AMyDebugTextAndColorsOnTiles::StaticClass());
	MyDebugTextAndColorsOnTiles = Cast<AMyDebugTextAndColorsOnTiles>(actor);	
}

void UMyTab_PathFinding::NativeDestruct()
{
	Super::NativeDestruct();
	
}

void UMyTab_PathFinding::OnCheckIndexesChanged(bool isChecked)
{
	ShowTilesIndexes = isChecked;
	MyDebugTextAndColorsOnTiles->SetShowTileText(ShowTilesIndexes);
}
