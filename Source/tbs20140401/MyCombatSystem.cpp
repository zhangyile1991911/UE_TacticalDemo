// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCombatSystem.h"

#include "Grid.h"
#include "MyUnit.h"
#include "My_Pawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyCombatSystem::AMyCombatSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
}

// Called when the game starts or when spawned
void AMyCombatSystem::BeginPlay()
{
	Super::BeginPlay();
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(),AGrid::StaticClass());
	MyGrid = Cast<AGrid>(actor);
}

TObjectPtr<AMyUnit>  AMyCombatSystem::SortActionPriority()
{
	if(UnitsActionPriority.IsEmpty())return nullptr;
	if(UnitsActionPriority.Num() <= 1)
	{
		FirstUnit = UnitsActionPriority[0];
		ReSortEvent.Broadcast(UnitsActionPriority);
		return FirstUnit;
	}
		

	bool WhoCanMove = false;
	while(!WhoCanMove)
	{
		for(auto Unit : UnitsActionPriority)
		{
			Unit->AddCurrentDistanceToAction();
			if(Unit->DistanceToAction() >= 1)
			{
				WhoCanMove = true;
				break;
			}
		}
	}
	
	UnitsActionPriority.Sort([](const TObjectPtr<AMyUnit>& L,const TObjectPtr<AMyUnit>& R)
	{//昇順　降順
		int a = L->DistanceToAction();
		int b = R->DistanceToAction();
		if(a == b)
		{
			return L->GetCurrentDistanceToAction() > R->GetCurrentDistanceToAction();
		}
		return a > b;
	});

	ReSortEvent.Broadcast(UnitsActionPriority);
	
	return FirstUnit = UnitsActionPriority[0];
}

// Called every frame
void AMyCombatSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyCombatSystem::AddUnitInCombat(const FIntPoint& Index, TObjectPtr<AMy_Pawn> myPawn)
{
	AActor* actor = GetWorld()->SpawnActor(AMyUnit::StaticClass());
	TObjectPtr<AMyUnit> Unit = Cast<AMyUnit>(actor);
	
	auto pData = MyGrid->GetTileDataByIndex(Index);
	Unit->SetActorLocation(pData->Transform.GetLocation());

	UnitsInCombat.Add(Index,Unit);
	UnitsActionPriority.Add(Unit);
	
	MyGrid->AddTileDataUnitByIndex(Index,Unit);

	Unit->RefreshUnit(myPawn,MyGrid,Index);
}

void AMyCombatSystem::RemoveUnitInCombat(const FIntPoint& Index)
{
	if(UnitsInCombat.Contains(Index))
	{
		UnitsActionPriority.Remove(UnitsInCombat[Index]);
		UnitsInCombat.Remove(Index);
	}
	
	MyGrid->RemoveTileDataUnitByIndex(Index);
}

void AMyCombatSystem::SetUnitIndexOnGrid(const FIntPoint& Index, TObjectPtr<AMyUnit> Unit)
{
	if(Index == Unit->GetGridIndex())
		return;
	const FTileData* pData = MyGrid->GetTileDataByIndex(Unit->GetGridIndex());	

	if(pData == nullptr)return;
	
	if(pData->UnitOnTile != nullptr)return;

	Unit->SetActorLocation(pData->Transform.GetLocation());
	
}

void AMyCombatSystem::NextAllUnit()
{
	for(auto Unit : UnitsActionPriority)
	{
		Unit->AddCurrentDistanceToAction();
	}
}

void AMyCombatSystem::ResetAllUnit()
{
	for(auto Unit : UnitsActionPriority)
	{
		Unit->ResetCurrentDistanceToAction();
	}
}


