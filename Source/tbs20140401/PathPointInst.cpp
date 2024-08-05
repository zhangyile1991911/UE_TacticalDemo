// Fill out your copyright notice in the Description page of Project Settings.


#include "PathPointInst.h"

#include "Grid.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"


// Sets default values
APathPointInst::APathPointInst()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    InstancedStaticMeshComponent = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("UInstancedStaticMeshComponent"));
    InstancedStaticMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APathPointInst::BeginPlay()
{
	Super::BeginPlay();

	if(PointMesh.IsValid() == false)
	{
		PointMesh.LoadSynchronous();
	}
	InstancedStaticMeshComponent->SetStaticMesh(PointMesh.Get());
	
	if(PointMaterial.IsValid() == false)
	{
		PointMaterial.LoadSynchronous();
	}
	// InstancedStaticMeshComponent->SetMaterial(0,PointIM.Get());
	// UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(PointMaterial.Get(), this);
	// if (DynamicMaterial)
	// {
	// 	// 设置材质参数
	// 	DynamicMaterial->SetVectorParameterValue("Color", FLinearColor::Red);
 //            
	// 	// 设置动态材质实例
	// 	InstancedStaticMeshComponent->SetMaterial(0, DynamicMaterial);
	// }
	InstancedStaticMeshComponent->SetVectorParameterValueOnMaterials(TEXT("Color"),FVector::Zero());
	InstancedStaticMeshComponent->SetMaterial(0, PointMaterial.Get());
	InstancedStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// InstancedStaticMeshComponent->SetBoundsScale(9999999);
	// InstancedStaticMeshComponent->SetCullDistances(999999,1999999999);
	
	InstancedStaticMeshComponent->bAffectDistanceFieldLighting = false;
	InstancedStaticMeshComponent->bCastDynamicShadow = false;
	InstancedStaticMeshComponent->bCastStaticShadow = false;
	InstancedStaticMeshComponent->bReceivesDecals = false;
	InstancedStaticMeshComponent->bNeverDistanceCull = true; // 防止剔除
	InstancedStaticMeshComponent->bEnableDensityScaling = false; 
	
	ArrayOfMeshInstPool.Reserve(64);
	//事例を作成しとく
	CreateShowPoint(16);
	UsedIndex = 0;
}

void APathPointInst::CreateShowPoint(int Num)
{
	TArray<FTransform> Tmp;
	for(int i = 0;i < Num;i++)
	{
		FTransform tf;
		tf.SetScale3D(FVector::Zero());
		Tmp.Add(tf);
	}
	auto AddIndex = InstancedStaticMeshComponent->AddInstances(Tmp,true,true);
	ArrayOfMeshInstPool.Append(AddIndex);
}

int APathPointInst::TakeMeshInstIndex()
{
	if(UsedIndex >= ArrayOfMeshInstPool.Num())
	{
		CreateShowPoint(16);
	}
	return ArrayOfMeshInstPool[UsedIndex++];
}

void APathPointInst::RecycleInstIndex()
{
	FTransform tf;
	tf.SetScale3D(FVector::Zero());
	InstancedStaticMeshComponent->BatchUpdateInstancesTransform(0,UsedIndex,tf);
	UsedIndex = 0;
}

// Called every frame
void APathPointInst::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APathPointInst::HidePathPoint()
{
	RecycleInstIndex();
}

void APathPointInst::ShowPathPoint(const TArray<FIntPoint>& Path, TObjectPtr<AGrid> MyGrid)
{
	RecycleInstIndex();
	if(Path.IsEmpty())return;
	
	for(int i = 0;i < Path.Num()-1;i++)
	{
		const FIntPoint& First = Path[i];
		const FIntPoint& Second = Path[i+1];
		FVector FirstVec = MyGrid->GetTileLocationFromGridIndex(First);	
		FVector SecondVec = MyGrid->GetTileLocationFromGridIndex(Second);
		//距離を計算する
		const float Dist = FVector::Distance(FirstVec,SecondVec);
		//必要な数量
		const int PointNum = Dist/PointGap;
		//方向を決めます
		FVector Dir = SecondVec - FirstVec;
		Dir.Normalize();

		for(int p = 0;p < PointNum;p++)
		{
			FVector Position = FirstVec + Dir * PointGap * p;
			Position.Z += 20;
			FTransform tf;
			tf.SetLocation(Position);
			tf.SetRotation(FQuat(FRotator(0,45,0)));
			tf.SetScale3D(FVector(0.35f,0.35f,0.1f));
			
			int InstIndex = TakeMeshInstIndex();
			// UE_LOG(LogTemp,Log,TEXT("for(int p = 0;p < PointNum;p++) %s %d"),*Position.ToString(),InstIndex)
			InstancedStaticMeshComponent->UpdateInstanceTransform(InstIndex,tf,true,false);	
		}
	}
	//最後の道印を置く
	int InstIndex = TakeMeshInstIndex();
	const FIntPoint& Last = Path.Last();
	FVector Position = MyGrid->GetTileLocationFromGridIndex(Last);
	Position.Z += 20;
	// UE_LOG(LogTemp,Log,TEXT("最後の道印を置く %s %d"),*Position.ToString(),InstIndex)
	// Position += Dir * PointGap;
	FTransform tf;
	tf.SetLocation(Position);
	tf.SetRotation(FQuat(FRotator(0,45,0)));
	tf.SetScale3D(FVector(0.35f,0.35f,0.1f));
	InstancedStaticMeshComponent->UpdateInstanceTransform(InstIndex,tf,true,true);
}

