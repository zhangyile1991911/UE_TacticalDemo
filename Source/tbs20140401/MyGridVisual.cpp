// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGridVisual.h"

#include "Grid.h"
#include "MyGridMeshInst.h"
#include "My_Utilities.h"
#include "TileData.h"

// Sets default values
AMyGridVisual::AMyGridVisual()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("scene"));
	ChildActor_LayerA = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor_LayerA"));
	ChildActor_LayerA->SetupAttachment(RootComponent);
	ChildActor_LayerA->SetChildActorClass(AMyGridMeshInst::StaticClass());

	ChildActor_LayerB = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor_LayerB"));
	ChildActor_LayerB->SetupAttachment(RootComponent);
	ChildActor_LayerB->SetChildActorClass(AMyGridMeshInst::StaticClass());

	// UE_LOG(LogTemp,Log,TEXT("AMyGridVisual::AMyGridVisual default construct %s at %s thread:  %d"),*GetName(),*FDateTime::Now().ToString(), FPlatformTLS::GetCurrentThreadId())
}

// Called when the game starts or when spawned
void AMyGridVisual::BeginPlay()
{
	Super::BeginPlay();

}

void AMyGridVisual::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

// Called every frame
void AMyGridVisual::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyGridVisual::InitializedGridVisual(AGrid* grid)
{
	myGrid = grid;
	FGridShapeData* data = myGrid->GetGridShape();
	if(!data->FlatMesh.IsValid())
		data->FlatMesh.LoadSynchronous();
	
	if(!data->FlatMaterial.IsValid())
		data->FlatMaterial.LoadSynchronous();

	ChildActor_LayerA->CreateChildActor([this,data](AActor* Actor)
	{
		LayerAGridMeshInst = Cast<AMyGridMeshInst>(Actor);	
		LayerAGridMeshInst->InitializeGridMeshInst(data->FlatMesh.Get(),data->FlatMaterial.Get(),FVector(0,0,0),ECollisionEnabled::Type::QueryOnly);
		LayerAGridMeshInst->AddShowTileTypes(ETileState::Reachable);
		LayerAGridMeshInst->AddShowTileTypes(ETileState::DangerousRange);
		// LayerAGridMeshInst->AddShowTileTypes(ETileState::Selected);
		// LayerAGridMeshInst->AddShowTileTypes(ETileState::Hovered);
	});
	ChildActor_LayerB->SetRelativeLocation(FVector::Zero());
	
	ChildActor_LayerB->CreateChildActor([this,data](AActor* Actor)
	{
		LayerBGridMeshInst = Cast<AMyGridMeshInst>(Actor);	
		LayerBGridMeshInst->InitializeGridMeshInst(data->FlatMesh.Get(),data->FlatMaterial.Get(),FVector(0,0,0),ECollisionEnabled::Type::QueryOnly);
		LayerBGridMeshInst->AddShowTileTypes(ETileState::PathFinding);
		LayerBGridMeshInst->AddShowTileTypes(ETileState::Selected);
		LayerBGridMeshInst->AddShowTileTypes(ETileState::Hovered);
		LayerBGridMeshInst->AddShowTileTypes(ETileState::IndicatorRange);
		LayerBGridMeshInst->AddShowTileTypes(ETileState::AbilityRange);
		LayerBGridMeshInst->AddShowTileTypes(ETileState::OtherUnitWalkRange);
		LayerBGridMeshInst->AddShowTileTypes(ETileState::OtherUnitAssaultRange);
	});
	ChildActor_LayerB->SetRelativeLocation(FVector(0,0,1.0f));
	SetActorLocation(FVector::Zero());
	
}

// void AMyGridVisual::SetOffsetFromGround(float offset)
// {
// 	offsetFromGround = offset;
// 	auto location = GetActorLocation();
// 	location.Z = offset;
// 	SetActorLocation(location);
// }

void AMyGridVisual::DestroyGridVisual()
{
	if(LayerAGridMeshInst != nullptr)
		LayerAGridMeshInst->ClearInstance();
	if(LayerBGridMeshInst != nullptr)
		LayerBGridMeshInst->ClearInstance();
}

void AMyGridVisual::AddTileVisual(FTileData data)
{
	if(IsTileTypeWalkable(data.TileType))
	{
		// UE_LOG(LogTemp,Log,TEXT("AMyGridVisual::UpdateTileVisual add x = %d y = %d"),data.Index.X,data.Index.Y)
		// myGridMeshInst->UpdateInstance(data.Transform,data.Index,data.States);
		// for(auto OneState : data.States)
		// {
		// 	if(LayerAGridMeshInst->ContainShowTileType(OneState))
		// 	{
		// 		LayerAGridMeshInst->AddInstance(data.Transform,data.Index,data.States);	
		// 	}
		// 	if(LayerBGridMeshInst->ContainShowTileType(OneState))
		// 	{
		// 		FTransform Other = data.Transform;
		// 		FVector Location = Other.GetLocation();
		// 		Location.Z += 5.0f;
		// 		Other.SetLocation(Location);
		// 		LayerBGridMeshInst->AddInstance(Other,data.Index,data.States);	
		// 	}	
		// }
		LayerAGridMeshInst->AddInstance(data.Transform,data.Index,data.States);
		FTransform Other = data.Transform;
		FVector Location = Other.GetLocation();
		Location.Z += 5.0f;
		Other.SetLocation(Location);
		LayerBGridMeshInst->AddInstance(Other,data.Index,data.States);	
	}
}


void AMyGridVisual::UpdateTileVisual(FTileData data,EGriUpdateMode mode)//0 state 1 tiletype 2 transform
{
	// if(myGridMeshInst != nullptr)
	// {
	// 	// UE_LOG(LogTemp,Log,TEXT("AMyGridVisual::UpdateTileVisual remove x = %d y = %d"),data.Index.X,data.Index.Y)
	// 	myGridMeshInst->RemoveInstance(data.Index);	
	// }
	if(data.TileType == ETileType::None)
	{
		LayerAGridMeshInst->RemoveInstance(data.Index);
		LayerBGridMeshInst->RemoveInstance(data.Index);
		return;
	}
	if(mode == EGriUpdateMode::UpdateState)
	{
		if(IsTileTypeWalkable(data.TileType))
		{
			// UE_LOG(LogTemp,Log,TEXT("AMyGridVisual::UpdateTileVisual add x = %d y = %d"),data.Index.X,data.Index.Y)
			// myGridMeshInst->UpdateInstance(data.Transform,data.Index,data.States);
			LayerAGridMeshInst->UpdateInstance(data.Index,data.States);
			LayerBGridMeshInst->UpdateInstance(data.Index,data.States);
		}	
	}

	if(mode == EGriUpdateMode::UpdateTileType)
	{
		LayerAGridMeshInst->UpdateInstance(data.Index,data.States);
		LayerBGridMeshInst->UpdateInstance(data.Index,data.States);
	}

	if(mode == EGriUpdateMode::UpdateTransform)
	{
		LayerAGridMeshInst->UpdateInstanceTransform(data.Index,data.Transform);
		LayerBGridMeshInst->UpdateInstanceTransform(data.Index,data.Transform);
	}
	
}

void AMyGridVisual::RemoveTile(FIntPoint index)
{
	LayerAGridMeshInst->RemoveInstance(index);
	LayerBGridMeshInst->RemoveInstance(index);
}
