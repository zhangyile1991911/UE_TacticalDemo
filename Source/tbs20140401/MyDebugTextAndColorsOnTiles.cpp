// Fill out your copyright notice in the Description page of Project Settings.


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
	IsShowIndex = ShowIndex;
	if(ShowIndex)
	{
		UpdateStateOnTile(ShowIndex);
	}
}

void AMyDebugTextAndColorsOnTiles::ShowDebugInfos(TArray<FMyPathFindingData> grids, bool ShowCost, bool ShowStart,
	bool ShowTarget)
{
	for(const FMyPathFindingData& one : grids)
	{
		auto actor = GetTextActor(one.Index);

		const FTileData* pTileData = myGrid->GetTileDataByIndex(one.Index);
		FString DebugText;
		DebugText.Appendf(TEXT("(%d,%d)"),one.Index.X,one.Index.Y);
		if(ShowCost)
		{
			DebugText.Appendf(TEXT("\nCost:%d"),one.CostFromStart+one.MinimumCostToTarget+one.CostToEnterTile);
		}
		if(ShowStart)
		{
			DebugText.Appendf(TEXT("\nStart:%d"),one.CostFromStart);
		}
		if(ShowTarget)
		{
			DebugText.Appendf(TEXT("\nTarget:%d"),one.MinimumCostToTarget);
		}
		FVector pos = pTileData->Transform.GetLocation();
		pos.Z += 10.0f;
		actor->GetTextRender()->SetText(FText::FromString(DebugText));
		actor->GetTextRender()->TextRenderColor = FColor::Red;
		actor->SetActorLocation(pos);
		actor->SetActorRotation(FRotator(90,180,0));
	}
}

void AMyDebugTextAndColorsOnTiles::ShowDebugInfo(const FMyPathFindingData& FindingData, bool ShowCost, bool ShowStart,
	bool ShowTarget)
{
	auto actor = GetTextActor(FindingData.Index);

	const FTileData* pTileData = myGrid->GetTileDataByIndex(FindingData.Index);
	FString DebugText;
	DebugText.Appendf(TEXT("(%d,%d)"),FindingData.Index.X,FindingData.Index.Y);
	if(ShowCost)
	{
		DebugText.Appendf(TEXT("\nCost:%d"),FindingData.CostFromStart+FindingData.MinimumCostToTarget+FindingData.CostToEnterTile);
	}
	if(ShowStart)
	{
		DebugText.Appendf(TEXT("\nStart:%d"),FindingData.CostFromStart);
	}
	if(ShowTarget)
	{
		DebugText.Appendf(TEXT("\nTarget:%d"),FindingData.MinimumCostToTarget);
	}
	FVector pos = pTileData->Transform.GetLocation();
	pos.Z += 10.0f;
	actor->GetTextRender()->SetText(FText::FromString(DebugText));
	actor->GetTextRender()->TextRenderColor = FColor::Red;
	actor->SetActorLocation(pos);
	actor->SetActorLocation(pos);
	actor->SetActorRotation(FRotator(90,180,0));
}

void AMyDebugTextAndColorsOnTiles::ClearDebugInfo(const FMyPathFindingData& FindingData)
{
	auto actor = GetTextActor(FindingData.Index);
	if(IsShowIndex)
	{
		actor->GetTextRender()->SetText(FText::Format(
			FText::FromString(TEXT("({0},{1})")),
			FText::AsNumber(FindingData.Index.X),
			FText::AsNumber(FindingData.Index.Y)));	
	}
	else
	{
		actor->GetTextRender()->SetText(FText::GetEmpty());	
	}
}
