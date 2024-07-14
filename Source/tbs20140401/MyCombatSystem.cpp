// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCombatSystem.h"

#include "AssaultTips.h"
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
	
	static ConstructorHelpers::FClassFinder<AAssaultTips> AssaultTips(TEXT("/Game/Art/Units/BP_AssaultTips"));
	if (AssaultTips.Succeeded())
	{
		AssaultTipsClass = AssaultTips.Class;
	}
}

// Called when the game starts or when spawned
void AMyCombatSystem::BeginPlay()
{
	Super::BeginPlay();
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(),AGrid::StaticClass());
	MyGrid = Cast<AGrid>(actor);

	for(int i = 0;i < 5;i++)
	{
		CreateOneTips();	
	}
	
}

void AMyCombatSystem::CreateOneTips()
{
	auto CurWorld = GetWorld();

	auto Actor = CurWorld->SpawnActor(AssaultTipsClass);
	auto Tips = Cast<AAssaultTips>(Actor);
	Tips->SetActorHiddenInGame(true);
	TipsObjectPool.Add(Tips);
	Tips->StopAnimation();
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

float CalculateDistance(const FIntPoint& Point1, const FIntPoint& Point2)
{
	// 计算两个点之间的距离
	return FMath::Sqrt(FMath::Square(float(Point1.X - Point2.X)) + FMath::Square(float(Point1.Y - Point2.Y)));
}

TArray<TObjectPtr<AMyUnit>> AMyCombatSystem::GetThreatenEnemies(TObjectPtr<AMyUnit> Unit)
{
	TArray<TObjectPtr<AMyUnit>> Result;
	FIntPoint UnitIndex = Unit->GetGridIndex();
	for(const auto& pair: UnitsInCombat)
	{
		auto Value = pair.Value;
		if(Unit->IsFriend(Value->GetUnitSide()))continue;

		FIntPoint EnemyIndex = Value->GetGridIndex();
		float Distance = CalculateDistance(UnitIndex,EnemyIndex);
		if(Distance > Value->GetMaxAtkRange() + Unit->GetMove())continue;
		
		Result.Add(Value);
	}
	return MoveTemp(Result);
}

void AMyCombatSystem::ShowUnitThreaten(TArray<FThreatenInfo> Infos)
{
	HideUnitThreaten();
	if(Infos.Num() > TipsObjectPool.Num())
	{
		int AddNum = Infos.Num() - TipsObjectPool.Num();
		for(int i = 0;i < AddNum;i++)
		{
			CreateOneTips();	
		}
	}
	for(int i = 0;i < Infos.Num();i++)
	{
		TipsObjectPool[i]->DoAnimation(Infos[i].Attacker,
			Infos[i].Defender);
		TipsObjectPool[i]->SetActorHiddenInGame(false);
	}
}

void AMyCombatSystem::HideUnitThreaten()
{
	for(auto Tips : TipsObjectPool)
	{
		Tips->StopAnimation();
		Tips->SetActorHiddenInGame(true);
	}
}

TArray<TObjectPtr<AMyUnit>> AMyCombatSystem::GetOneSideTeam(int UnitSide)
{
	TArray<TObjectPtr<AMyUnit>> Result;
	for(const auto& one : UnitsInCombat)
	{
		if(one.Value->GetUnitSide() == UnitSide)
			Result.Add(one.Value);
	}
	return Result;
}

void AMyCombatSystem::ClearAllUnit()
{
	FirstUnit = nullptr;

	UnitsActionPriority.Empty();
	HideUnitThreaten();
	for(const auto& Pair : UnitsInCombat)
	{
		Pair.Value->Destroy();
	}
	UnitsInCombat.Empty();
}

TObjectPtr<AMyUnit> AMyCombatSystem::GetUnitByType(ETBSUnitType EtbsUnit)
{
	for(auto one : UnitsInCombat)
	{
		if(one.Value->GetUnitType() == EtbsUnit)
		{
			return one.Value;
		}
	}
	return nullptr;
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

	UnitsInCombat.Add(Unit->GetUniqueID(),Unit);
	UnitsActionPriority.Add(Unit);
	
	MyGrid->AddTileDataUnitByIndex(Index,Unit);

	Unit->RefreshUnit(myPawn,MyGrid,Index);
}

TObjectPtr<AMyUnit> AMyCombatSystem::AddUnitInCombatByType(const FIntPoint& Index, ETBSUnitType UT,
	EUnitDirectType Direction, AMy_Pawn* MyPawn)
{
	AActor* actor = GetWorld()->SpawnActor(AMyUnit::StaticClass());
	TObjectPtr<AMyUnit> Unit = Cast<AMyUnit>(actor);
	
	auto pData = MyGrid->GetTileDataByIndex(Index);
	Unit->SetActorLocation(pData->Transform.GetLocation());

	UnitsInCombat.Add(Unit->GetUniqueID(),Unit);
	UnitsActionPriority.Add(Unit);
	
	MyGrid->AddTileDataUnitByIndex(Index,Unit);
	Unit->RefreshUnitFromLoad(Index,UT,Direction,MyPawn,MyGrid);

	return Unit;	
}

void AMyCombatSystem::RemoveUnitInCombat(const FIntPoint& Index)
{
	const FTileData* TileData = MyGrid->GetTileDataByIndex(Index);
	if(TileData == nullptr || TileData->UnitOnTile == nullptr)return;
	
	RemoveUnitInCombat(TileData->UnitOnTile);
}

void AMyCombatSystem::RemoveUnitInCombat(TObjectPtr<AMyUnit> Unit)
{
	uint32 id = Unit->GetUniqueID();
	if(UnitsInCombat.Contains(id))
	{
		UnitsActionPriority.Remove(UnitsInCombat[id]);
		UnitsInCombat.Remove(id);
	}
	
	MyGrid->RemoveTileDataUnitByIndex(Unit->GetGridIndex());
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


