// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowUnit.h"

#include "MyUnit.h"
#include "My_Utilities.h"


// Sets default values
AShadowUnit::AShadowUnit()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShadowMesh"));
	SkeletalMeshComponent->SetupAttachment(this->RootComponent);

	SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
}

// Called when the game starts or when spawned
void AShadowUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShadowUnit::RefreshUnit(TObjectPtr<AMyUnit> Master,UClass* AnimBP)
{
	MyMaster = Master;
	EUnitType UnitType = MyMaster->GetUnitType();
	auto pData = GetUnitData(UnitType);
	if(pData == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("%d data is null"),UnitType);
		return;
	}

	if(pData->Assets.SkeletalMesh.IsValid())
	{
		SkeletalMeshComponent->SetSkeletalMesh(pData->Assets.SkeletalMesh.Get());
	}
	else
	{
		auto skeletalMesh = pData->Assets.SkeletalMesh.LoadSynchronous();
		SkeletalMeshComponent->SetSkeletalMesh(skeletalMesh);
	}
	SkeletalMeshComponent->SetAnimInstanceClass(AnimBP);
	// SetActorRotation(FRotator(0,360-90,0));

	SkeletalMeshComponent->SetVectorParameterValueOnMaterials(TEXT("ColorMultiply"),FVector(1.0f,1.0f,1.0f));
}

void AShadowUnit::TurnLeft()
{
}

void AShadowUnit::TurnRight()
{
}

void AShadowUnit::TurnForward()
{
}

void AShadowUnit::TurnBack()
{
}

// Called every frame
void AShadowUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

