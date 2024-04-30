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
	ChildActor_GridInstMesh = CreateDefaultSubobject<UChildActorComponent>(TEXT("childActor"));
	ChildActor_GridInstMesh->SetupAttachment(RootComponent);
	ChildActor_GridInstMesh->SetChildActorClass(AMyGridMeshInst::StaticClass());

	UE_LOG(LogTemp,Log,TEXT("AMyGridVisual::AMyGridVisual default construct %s at %s thread:  %d"),*GetName(),*FDateTime::Now().ToString(), FPlatformTLS::GetCurrentThreadId())
}

// Called when the game starts or when spawned
void AMyGridVisual::BeginPlay()
{
	Super::BeginPlay();
	myGridMeshInst = Cast<AMyGridMeshInst>(ChildActor_GridInstMesh->GetChildActor());
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
	
	myGridMeshInst = Cast<AMyGridMeshInst>(ChildActor_GridInstMesh->GetChildActor());
	myGridMeshInst->InitializeGridMeshInst(data->FlatMesh.Get(),data->FlatMaterial.Get(),FVector(0,0,0),ECollisionEnabled::Type::QueryOnly);
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
	if(myGridMeshInst != nullptr)
		myGridMeshInst->ClearInstance();
}

void AMyGridVisual::AddTileVisual(FTileData data)
{
	if(IsTileTypeWalkable(data.TileType))
	{
		// UE_LOG(LogTemp,Log,TEXT("AMyGridVisual::UpdateTileVisual add x = %d y = %d"),data.Index.X,data.Index.Y)
		// myGridMeshInst->UpdateInstance(data.Transform,data.Index,data.States);
		myGridMeshInst->AddInstance(data.Transform,data.Index,data.States);
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
		myGridMeshInst->RemoveInstance(data.Index);
		return;
	}
	if(mode == EGriUpdateMode::UpdateState)
	{
		if(IsTileTypeWalkable(data.TileType))
		{
			// UE_LOG(LogTemp,Log,TEXT("AMyGridVisual::UpdateTileVisual add x = %d y = %d"),data.Index.X,data.Index.Y)
			// myGridMeshInst->UpdateInstance(data.Transform,data.Index,data.States);
			myGridMeshInst->UpdateInstance(data.Index,data.States);
		}	
	}

	if(mode == EGriUpdateMode::UpdateTileType)
	{
		myGridMeshInst->UpdateInstance(data.Index,data.States);
	}

	if(mode == EGriUpdateMode::UpdateTransform)
	{
		myGridMeshInst->UpdateInstanceTransform(data.Index,data.Transform);
	}
	
}

void AMyGridVisual::RemoveTIle(FIntPoint index)
{
	myGridMeshInst->RemoveInstance(index);
}
