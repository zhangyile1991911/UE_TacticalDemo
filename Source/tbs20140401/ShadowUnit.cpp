// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowUnit.h"

#include "MyGameInstance.h"
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

void AShadowUnit::RefreshUnit(TObjectPtr<AMyUnit> Master,UClass* AnimBP,FUnitColorDataAsset* DataAssetPtr)
{
	MyMaster = Master;
	ETBSUnitType UnitType = MyMaster->GetUnitType();
	auto pData = Master->GetGameInstance()->GetUnitData(UnitType);
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

	if(DataAssetPtr == nullptr)return;
	
	if(!DataAssetPtr->Slot1.IsValid())
	{
		auto m1 = DataAssetPtr->Slot1.LoadSynchronous();
		SkeletalMeshComponent->SetMaterial(0,m1);
	}
	else
	{
		SkeletalMeshComponent->SetMaterial(0,DataAssetPtr->Slot1.Get());
	}
	
	if(!DataAssetPtr->Slot2.IsValid())
	{
		auto m1 = DataAssetPtr->Slot2.LoadSynchronous();
		SkeletalMeshComponent->SetMaterial(1,m1);
	}
	else
	{
		SkeletalMeshComponent->SetMaterial(1,DataAssetPtr->Slot2.Get());
	}
	
	if(!DataAssetPtr->Slot3.IsValid())
	{
		auto m1 = DataAssetPtr->Slot3.LoadSynchronous();
		SkeletalMeshComponent->SetMaterial(2,m1);
	}
	else
	{
		SkeletalMeshComponent->SetMaterial(2,DataAssetPtr->Slot3.Get());
	}
	
	if(!DataAssetPtr->Slot4.IsValid())
	{
		auto m1 = DataAssetPtr->Slot4.LoadSynchronous();
		SkeletalMeshComponent->SetMaterial(3,m1);
	}
	else
	{
		SkeletalMeshComponent->SetMaterial(3,DataAssetPtr->Slot4.Get());
	}
	
	if(!DataAssetPtr->Slot5.IsValid())
	{
		auto m1 = DataAssetPtr->Slot5.LoadSynchronous();
		SkeletalMeshComponent->SetMaterial(4,m1);
	}
	else
	{
		SkeletalMeshComponent->SetMaterial(4,DataAssetPtr->Slot5.Get());
	}
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

