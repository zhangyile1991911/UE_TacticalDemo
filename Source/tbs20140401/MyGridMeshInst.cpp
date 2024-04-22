// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGridMeshInst.h"

#include "GridShapeData.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
AMyGridMeshInst::AMyGridMeshInst()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("UInstancedStaticMeshComponent"));
	InstancedStaticMeshComponent->SetupAttachment(RootComponent);
	// InstancedStaticMeshComponent->SetNumCustomDataFloats(4);
	// RootComponent = InstancedStaticMeshComponent;
	UE_LOG(LogTemp,Log,TEXT("AMyGridMeshInst::AMyGridMeshInst() default construct %s at %s,thread:  %d"),*GetName(),*FDateTime::Now().ToString(), FPlatformTLS::GetCurrentThreadId())
}

// Called when the game starts or when spawned
void AMyGridMeshInst::BeginPlay()
{
	Super::BeginPlay();
	InstancedStaticMeshComponent->SetNumCustomDataFloats(4);
}

// Called every frame
void AMyGridMeshInst::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyGridMeshInst::AddInstance(FTransform& instancedTransform,FIntPoint index,const TArray<ETileState>& states)
{
	RemoveInstance(index);
	auto instIndex = InstancedStaticMeshComponent->AddInstance(instancedTransform);
	int one = InstancedIndexes.Add(index);
	UE_LOG(LogTemp,Log,TEXT("AMyGridMeshInst::AddInstance local = (%d,%d) instIndex = %d one = %d"),index.X,index.Y,instIndex,one);
	// float filled = 0;
	// FLinearColor color = GetColorFromStates(states,filled);
	// InstancedStaticMeshComponent->SetCustomDataValue(one,0,color.R);
	// InstancedStaticMeshComponent->SetCustomDataValue(one,1,color.G);
	// InstancedStaticMeshComponent->SetCustomDataValue(one,2,color.B);
	// InstancedStaticMeshComponent->SetCustomDataValue(one,3,filled);
}

void AMyGridMeshInst::RemoveInstance(FIntPoint index)
{
	auto result = InstancedIndexes.Find(index);
	if(result != INDEX_NONE)
	{
		UE_LOG(LogTemp,Log,TEXT("AMyGridMeshInst::RemoveInstance local = (%d,%d) index = %d "),index.X,index.Y,result);
		InstancedStaticMeshComponent->RemoveInstance(result);
		InstancedIndexes.Remove(index);
	}
}

void AMyGridMeshInst::UpdateInstance(FIntPoint index,const TArray<ETileState>& states)
{
	auto result = InstancedIndexes.Find(index);
	if(result != INDEX_NONE)
	{
		
		float filled = 0;
		FLinearColor color = GetColorFromStates(states,filled);
		UE_LOG(LogTemp,Log,TEXT("AMyGridMeshInst::UpdateInstance local = (%d,%d) result = %d 1"),index.X,index.Y,result);
		InstancedStaticMeshComponent->SetCustomDataValue(result,0,color.R,true);
		InstancedStaticMeshComponent->SetCustomDataValue(result,1,color.G,true);
		InstancedStaticMeshComponent->SetCustomDataValue(result,2,color.B,true);
		InstancedStaticMeshComponent->SetCustomDataValue(result,3,filled,true);
		// InstancedStaticMeshComponent->MarkRenderInstancesDirty();
		
	}
	
}

FLinearColor AMyGridMeshInst::GetColorFromStates(const TArray<ETileState>& states,float& isFilled)
{
	isFilled = 0;
	if(states.IsEmpty())
	{
		return FLinearColor::Black;
	}
	for(auto s : states)
	{
		 if(s == ETileState::Selected)
		 {
		 	isFilled = 1.0f;
		 	return FLinearColor(0.212f,1.0f,0.098f); 
		 }
		 	
		if(s == ETileState::Hovered)
		{
			isFilled = 1.0f;
			return FLinearColor(0.55f,0.75f,0.351f);
		}
	}
	return FLinearColor::Black;
}

void AMyGridMeshInst::ClearInstance()
{
	InstancedIndexes.Empty();
	InstancedStaticMeshComponent->ClearInstances();
}

void AMyGridMeshInst::InitializeGridMeshInst(UStaticMesh* mesh,UMaterialInstance* mi,FVector color,ECollisionEnabled::Type ce)
{
	InstancedStaticMeshComponent->SetStaticMesh(mesh);
	InstancedStaticMeshComponent->SetMaterial(0,mi);
	InstancedStaticMeshComponent->SetVectorParameterValueOnMaterials(TEXT("Color"),color);
	InstancedStaticMeshComponent->SetCollisionEnabled(ce);
}