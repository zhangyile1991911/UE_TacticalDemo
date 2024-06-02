// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleDirection.h"

#include "Components/ArrowComponent.h"

// Sets default values
AIdleDirection::AIdleDirection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	// 确保组件在编辑器中可见
	Root->SetMobility(EComponentMobility::Movable);
	Root->SetVisibility(true);
	
	UPArrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UP"));
	UPArrow->SetupAttachment(RootComponent);
	
	LeftArrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	LeftArrow->SetupAttachment(RootComponent);

	DownArrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Down"));
	DownArrow->SetupAttachment(RootComponent);

	RightArrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	RightArrow->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane"));
	if (PlaneMeshAsset.Succeeded())
	{
		UPArrow->SetStaticMesh(PlaneMeshAsset.Object);
		LeftArrow->SetStaticMesh(PlaneMeshAsset.Object);
		DownArrow->SetStaticMesh(PlaneMeshAsset.Object);
		RightArrow->SetStaticMesh(PlaneMeshAsset.Object);
	}
	
	// RootComponent->SetRelativeRotation(FRotator(0,90,0));
}

void AIdleDirection::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
}

// Called when the game starts or when spawned
void AIdleDirection::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called every frame
void AIdleDirection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIdleDirection::ShowArrow()
{
	bShow = true;
	// RootComponent->SetVisibility(true);
}

void AIdleDirection::HideArrow()
{
	bShow = false;
	// RootComponent->SetVisibility(false);
}
