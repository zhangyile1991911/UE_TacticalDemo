// Fill out your copyright notice in the Description page of Project Settings.


#include "AssaultTips.h"


// Sets default values
AAssaultTips::AAssaultTips()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAssaultTips::BeginPlay()
{
	Super::BeginPlay();
	BezierPath.Reserve(BezierPathNum);
}

// Called every frame
void AAssaultTips::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAssaultTips::GenerateBezierPath(FVector Start, FVector Finish)
{
	BezierPath.Empty();
	
	FVector Dir = Finish - Start;
	Dir = Dir.GetSafeNormal();
	float fDistance = FVector::Distance(Start,Finish);
	FVector CtrlPoint = Start + Dir * fDistance / 2.0f;
	// CtrlPoint.Z += 800;
	CtrlPoint.Z += 500;
	for(int i = 1;i < BezierPathNum;i++)
	{
		float Per = float(i)/float(BezierPathNum);
		FVector one = GetQuadraticCurvePoint(Start,Finish,CtrlPoint,Per);
		BezierPath.Add(one);
	}
}

void AAssaultTips::UpdateBezierPath(float Val)
{
	const auto& first = BezierPath[BezierIndex];
	const auto& second = BezierPath[BezierIndex+1];
	const auto CurPos = FMath::Lerp(first,second,Val);
	SetActorLocation(CurPos);
}

void AAssaultTips::StartBezier()
{
	BezierIndex = 0;
}

bool AAssaultTips::FinishBezier()
{
	if(BezierIndex + 1 < BezierPath.Num() - 1)
	{
		BezierIndex += 1;
		return false;
	}
	return true;
}

