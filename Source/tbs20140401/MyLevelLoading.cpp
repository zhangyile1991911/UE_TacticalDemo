// Fill out your copyright notice in the Description page of Project Settings.


#include "MyLevelLoading.h"

#include "MovieSceneSequenceID.h"

// Sets default values
AMyLevelLoading::AMyLevelLoading()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
}

// Called when the game starts or when spawned
void AMyLevelLoading::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyLevelLoading::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyLevelLoading::LoadLevel(const FString& loadedLevel)
{
	if(curLevel == loadedLevel)return;
	
	for(auto ulevel : GetWorld()->GetStreamingLevels())
	{
		if(ulevel && ulevel->GetWorldAssetPackageName().EndsWith(curLevel))
		{
			ulevel->SetShouldBeLoaded(false);
			ulevel->SetShouldBeVisible(false);
			break;
		}
	}

	for(auto ulevel : GetWorld()->GetStreamingLevels())
	{
		if(ulevel && ulevel->GetWorldAssetPackageName().EndsWith(loadedLevel))
		{
			ulevel->SetShouldBeLoaded(true);
			ulevel->SetShouldBeVisible(true);
			break;
		}
	}
	curLevel = loadedLevel;
}

