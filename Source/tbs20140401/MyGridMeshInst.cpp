// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGridMeshInst.h"

#include "GridShapeData.h"
#include "IContentBrowserSingleton.h"
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

void AMyGridMeshInst::AddInstance(FTransform& instancedTransform,FIntPoint index,const TSet<ETileState>& states)
{
	RemoveInstance(index);
	auto instIndex = InstancedStaticMeshComponent->AddInstance(instancedTransform,true);
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

void AMyGridMeshInst::UpdateInstance(FIntPoint index,const TSet<ETileState>& states)
{
	auto result = InstancedIndexes.Find(index);
	if(result != INDEX_NONE)
	{
		
		float filled = 0;
		FLinearColor color = GetColorFromStates(states,filled);
		// UE_LOG(LogTemp,Log,TEXT("AMyGridMeshInst::UpdateInstance local = (%d,%d) result = %d 1"),index.X,index.Y,result);
		InstancedStaticMeshComponent->SetCustomDataValue(result,0,color.R,false);
		InstancedStaticMeshComponent->SetCustomDataValue(result,1,color.G,false);
		InstancedStaticMeshComponent->SetCustomDataValue(result,2,color.B,false);
		InstancedStaticMeshComponent->SetCustomDataValue(result,3,filled,true);
		// InstancedStaticMeshComponent->MarkRenderInstancesDirty();
	}
}

void AMyGridMeshInst::UpdateInstanceTransform(const FIntPoint& index,const FTransform& Transform)
{
	auto result = InstancedIndexes.Find(index);
	InstancedStaticMeshComponent->UpdateInstanceTransform(result,Transform,false,true);
}

FLinearColor AMyGridMeshInst::GetColorFromStates(const TSet<ETileState>& states,float& isFilled)
{
	isFilled = 0;
	if(states.IsEmpty())
	{
		return FLinearColor::Black;
	}

	const auto Result = states.Intersect(ShowTileTypes);
	if(Result.IsEmpty())return FLinearColor::Black;
	
	 if(Result.Contains(ETileState::Selected))
	 {
		isFilled = 1.0f;
		return FLinearColor(0.212f,1.0f,0.098f); 
	 }
		
	if(Result.Contains(ETileState::Hovered))
	{
		isFilled = 1.0f;
		return FLinearColor(0.55f,0.75f,0.351f);
	}

	if(Result.Contains(ETileState::OtherUnitWalkRange))
	{//みどり
		isFilled = 0.6f;
		return FLinearColor(0.3f,0.75f,0.251f);
	}
	if(Result.Contains(ETileState::OtherUnitAssaultRange))
	{//赤
		isFilled = 0.6f;
		return FLinearColor(0.75f,0.19f,0.129f);
	}
	// if(states.Contains(ETileState::IsNeighbor))
	// {
	// 	isFilled = 1.0f;
	// 	return FLinearColor::Yellow;
	// }
	if(Result.Contains(ETileState::DangerousRange))
	{
		isFilled = 1.0f;
		return FLinearColor(0.9f,0.07f,1.0f);
	}
	if(Result.Contains(ETileState::IndicatorRange))
	{
		isFilled = 1.0f;
		return FLinearColor(1.0f,0.4,0);
	}
	if(Result.Contains(ETileState::AbilityRange))
	{
		isFilled = 1.0f;
		return FLinearColor::Red;
	}

	if(Result.Contains(ETileState::PathFinding))
	{
		isFilled = 0.5f;
		return FLinearColor::White;
	}

	if(Result.Contains(ETileState::Reachable))
	{
		isFilled = 1.0f;
		return FLinearColor(0.019f,0.08f,0.25f);
	}

	return FLinearColor::Black;
}

void AMyGridMeshInst::AddShowTileTypes(ETileState TileState)
{
	if(ShowTileTypes.Contains(TileState))return;
	ShowTileTypes.Add(TileState);
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