// Fill out your copyright notice in the Description page of Project Settings.


#include "GridModifier.h"

#include "GridShapeData.h"
#include "My_Utilities.h"
#include "Components/BoxComponent.h"


// Sets default values
AGridModifier::AGridModifier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("static_mesh"));
	StaticMesh->SetupAttachment(RootComponent);
	// StaticMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Overlap);
	
	// BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	// BoxCollision->SetupAttachment(RootComponent);
	// BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	// BoxCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Overlap);
	
	RootComponent->SetMobility(EComponentMobility::Movable);
	AActor::SetActorHiddenInGame(true);
	// static ConstructorHelpers::FObjectFinder<UDataTable> DataTableFinder(TEXT("DataTable'/Game/Core/Grid/GridShapes/DT_GridShapeData.DT_GridShapeData'"));
	// if (DataTableFinder.Succeeded())
	// {
	// 	LoadedDataTable = DataTableFinder.Object;
	// }
	// else
	// {
	// 	UE_LOG(LogTemp,Log,TEXT("AGridModifier::OnConstruction load failed"))
	// 	return;
	// }
}

void AGridModifier::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if(Shape == EGridShape::None)return;
	
	// FVector ActorScale = Transform.GetScale3D();
	// FVector OriginalBoxExtent = FVector(50.f, 50.f, 50.f); // 原始尺寸
	// BoxCollision->InitBoxExtent(OriginalBoxExtent * ActorScale);

	FGridShapeData* row = GetShapeData(Shape);
	// switch (Shape)
	// {
	// case EGridShape::Hexagon:
	// 	row = LoadedDataTable->FindRow<FGridShapeData>(FName(TEXT("Hexagon")),"");
	// 	break;
	// case EGridShape::Square:
	// 	row = LoadedDataTable->FindRow<FGridShapeData>(FName(TEXT("Square")),"");
	// 	break;
	// case EGridShape::Triangle:
	// 	row = LoadedDataTable->FindRow<FGridShapeData>(FName(TEXT("Triangle")),"");
	// 	break;
	// }
	if(!row->Mesh.IsValid())
	{
		auto mesh = row->Mesh.LoadSynchronous();
		StaticMesh->SetStaticMesh(mesh);
	}
	else
	{
		StaticMesh->SetStaticMesh(row->Mesh.Get());	
	}
	if(!row->FlatMaterial.IsValid())
	{
		auto mat = row->FlatMaterial.LoadSynchronous();
		StaticMesh->SetMaterial(0,mat);
	}
	else
	{
		StaticMesh->SetMaterial(0,row->FlatMaterial.Get());
	}
	// StaticMesh->SetScalarParameterValueOnMaterials(TEXT("IsFilled"),0.5f);
	
	switch (Type)
	{
	case ETileType::Normal:
		StaticMesh->SetVectorParameterValueOnMaterials(TEXT("Color"),FVector(1.0f,1.0f,1.0f));
		break;
	case ETileType::Obstacle:
		StaticMesh->SetVectorParameterValueOnMaterials(TEXT("Color"),FVector(1.0f,0,0));
			// StaticMesh->SetCustomPrimitiveDataFloat(0,1.0f);
			// StaticMesh->SetCustomPrimitiveDataFloat(1,0);
			// StaticMesh->SetCustomPrimitiveDataFloat(2,0);
			// StaticMesh->SetCustomPrimitiveDataFloat(3,0.5f);
			break;
	case ETileType::FlyingUnitsOnly:
		StaticMesh->SetVectorParameterValueOnMaterials(TEXT("Color"),FVector(0.3f,0.92f,1.0f));
		break;
	case ETileType::DoubleCost:
		StaticMesh->SetVectorParameterValueOnMaterials(TEXT("Color"),FVector(1.0f,0.7389f,0.7289f));
		break;
	case ETileType::TripleCost:
		StaticMesh->SetVectorParameterValueOnMaterials(TEXT("Color"),FVector(1.0f,0.83f,0.165));
		break;
	}
	StaticMesh->SetScalarParameterValueOnMaterials(TEXT("IsFilled"),1.0f);
	StaticMesh->SetCollisionResponseToChannels(ECR_Ignore);
	StaticMesh->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	StaticMesh->SetCollisionResponseToChannel(ECC_Camera,ECR_Block);
	StaticMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel2,ECR_Overlap);
	if(Type == ETileType::Obstacle)
	{
		StaticMesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);
	}
	else
	{
		StaticMesh->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	}

	// SetActorHiddenInGame(true);
}

// Called when the game starts or when spawned
void AGridModifier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridModifier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

