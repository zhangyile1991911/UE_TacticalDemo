// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUnit.h"

#include "IMyUnitAnimation.h"
#include "My_Utilities.h"

// Sets default values
AMyUnit::AMyUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	MySkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("StaticMeshComponent"));
	MySkeletalMeshComponent->SetupAttachment(this->RootComponent);

	//加载资源
	// static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("/Game/Art/Units/Warrior/ABP_Warrior.ABP_Warrior_C"));
	// if (AnimBP.Succeeded())
	// {
	// 	AnimBPClass = AnimBP.Class.Get();
	// }
	//
	// static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshLoad(TEXT("/Script/Engine.SkeletalMesh'/Game/Art/Units/Warrior/SK_Warrior.SK_Warrior'"));
	// if(SkeletalMeshLoad.Succeeded())
	// {
	// 	SkeletalMeshAsset = SkeletalMeshLoad.Object;
	// }
	
}

void AMyUnit::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	RefreshUnit(UnitType);
}

// Called when the game starts or when spawned
void AMyUnit::BeginPlay()
{
	Super::BeginPlay();
	
	
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle,this,&AMyUnit::testloop,3.0f,true);
}

void AMyUnit::testloop()
{
	testAnimIndex += 1;
	testAnimIndex %= 6;
	auto inst = MySkeletalMeshComponent->GetAnimInstance();
	auto myinter = Cast<IIMyUnitAnimation>(inst);
	if(inst == nullptr
		 && myinter == nullptr)
	{
		return;
	}
	myinter->Execute_SetUnitAnimationState(inst,(EUnitAnimation)testAnimIndex);	
}

void AMyUnit::RefreshUnit(EUnitType LUnitType)
{
	UnitType = LUnitType;
	auto pData = GetUnitData(UnitType);
	if(pData == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("%d data is null"),UnitType);
		return;
	}

	if(pData->Assets.SkeletalMesh.IsValid())
	{
		MySkeletalMeshComponent->SetSkeletalMesh(pData->Assets.SkeletalMesh.Get());
		
	}
	else
	{
		auto skeletalMesh = pData->Assets.SkeletalMesh.LoadSynchronous();
		MySkeletalMeshComponent->SetSkeletalMesh(skeletalMesh);
	}
		
	if(pData->Assets.AnimBP.IsValid())
	{
		MySkeletalMeshComponent->SetAnimInstanceClass(pData->Assets.AnimBP.Get());
	}
	else
	{
		auto bp = pData->Assets.AnimBP.LoadSynchronous();
		MySkeletalMeshComponent->SetAnimInstanceClass(bp);
	}
	SetActorRotation(FRotator(0,-90,0));
}


// Called every frame
void AMyUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

