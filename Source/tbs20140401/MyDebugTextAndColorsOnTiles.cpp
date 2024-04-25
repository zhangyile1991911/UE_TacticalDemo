﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDebugTextAndColorsOnTiles.h"

#include "Grid.h"
#include "My_Utilities.h"
#include "Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMyDebugTextAndColorsOnTiles::AMyDebugTextAndColorsOnTiles()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMyDebugTextAndColorsOnTiles::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(),AGrid::StaticClass());
	myGrid = Cast<AGrid>(actor); 
	
}

// Called every frame
void AMyDebugTextAndColorsOnTiles::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ATextRenderActor* AMyDebugTextAndColorsOnTiles::GetTextActor(const FIntPoint& index)
{
	if(SpawnedTexts.Contains(index))
		return SpawnedTexts.FindChecked(index);

	AActor* actor = GetWorld()->SpawnActor(ATextRenderActor::StaticClass());
	auto one = Cast<ATextRenderActor>(actor);

	
	SpawnedTexts.Add(index,one);

	one->SetActorTickEnabled(true);
	one->SetActorEnableCollision(false);
	one->GetTextRender()->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	one->GetTextRender()->SetVerticalAlignment(EVRTA_TextCenter);
	
	return one;
	
}

void AMyDebugTextAndColorsOnTiles::DestroyTextActor(const FIntPoint& index)
{
	if(!SpawnedTexts.Contains(index))return;
	
	auto one = SpawnedTexts.FindChecked(index);
	if(one == nullptr)return;

	SpawnedTexts.Remove(index);
	one->Destroy();
	
}

void AMyDebugTextAndColorsOnTiles::ClearAllTextActors()
{
	for(auto one : SpawnedTexts)
	{
		one.Value->Destroy();
	}
	SpawnedTexts.Empty();
}


void AMyDebugTextAndColorsOnTiles::UpdateStateOnTile(const FIntPoint& index)
{
	UpdateTextOnAllTiles();
}

void AMyDebugTextAndColorsOnTiles::UpdateTextOnAllTiles()
{
	for(auto one : myGrid->GetGridTiles())
	{
		UpdateIndexTextOnTile(one.Key,one.Value);
		
	}
}

void AMyDebugTextAndColorsOnTiles::UpdateIndexTextOnTile(const FIntPoint& index,const FTileData& data)
{
	// const FTileData* pData = myGrid->GetTileDataByIndex(index);
	// if(pData == nullptr)return;
	if(IsTileTypeWalkable(data.TileType))
	{
		auto actor = GetTextActor(index);
		auto location = data.Transform.GetLocation();
		FText LocationText = FText::Format(
			FText::FromString("({0}, {1})"),
			FText::AsNumber(data.Index.X),
			FText::AsNumber(data.Index.Y)
		);
		location.Z += 10.0f;
		actor->GetTextRender()->SetText(LocationText);
		actor->SetActorLocation(location);
		actor->SetActorRotation(FRotator(90,180,0));
	}
	else
	{
		DestroyTextActor(index);
	}
}

void AMyDebugTextAndColorsOnTiles::SetShowTileText(bool ShowIndex)
{
	if(ShowIndex)
	{
		UpdateStateOnTile(ShowIndex);
	}
}