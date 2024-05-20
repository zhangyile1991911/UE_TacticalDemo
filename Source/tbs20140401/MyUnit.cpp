// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUnit.h"

#include "Grid.h"
#include "IMyUnitAnimation.h"
#include "My_Pawn.h"
#include "My_Utilities.h"
#include "Curves/CurveFloat.h"
#include "Components/TimelineComponent.h"

// Sets default values
AMyUnit::AMyUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	MySkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("StaticMeshComponent"));
	MySkeletalMeshComponent->SetupAttachment(this->RootComponent);

	CollisionEnabledHasQuery(ECollisionEnabled::QueryOnly);
	MySkeletalMeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3,ECR_Block);
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

	static ConstructorHelpers::FObjectFinder<UCurveFloat> LoCurve(TEXT("'/Game/Art/Units/LocationAlpha.LocationAlpha'"));
	if (LoCurve.Succeeded())
	{
		LocationCurve = LoCurve.Object;
	}
	static ConstructorHelpers::FObjectFinder<UCurveFloat> RoCurve(TEXT("'/Game/Art/Units/RotationAlpha.RotationAlpha'"));
	if (RoCurve.Succeeded())
	{
		RotationCurve = RoCurve.Object;
	}
	static ConstructorHelpers::FObjectFinder<UCurveFloat> JuCurve(TEXT("'/Game/Art/Units/JumpAlpha.JumpAlpha'"));
	if (JuCurve.Succeeded())
	{
		JumpCurve = JuCurve.Object;
	}
}

void AMyUnit::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	RefreshUnit(nullptr,nullptr,FIntPoint(0,0));
	
}

// Called when the game starts or when spawned
void AMyUnit::BeginPlay()
{
	Super::BeginPlay();
	
	if(LocationCurve)
	{
		FOnTimelineFloat tmp;
		tmp.BindDynamic(this,&AMyUnit::HandleLocationAlpha);
		UnitMovement.AddInterpFloat(LocationCurve,tmp);
		UnitMovement.SetLooping(false);
		UnitMovement.SetTimelineLengthMode(TL_LastKeyFrame);
		FOnTimelineEvent finish;
		finish.BindDynamic(this,&AMyUnit::FinishLocationAlpha);
		UnitMovement.SetTimelineFinishedFunc(finish);
	}
	if(RotationCurve)
	{
		FOnTimelineFloat tmp;
		tmp.BindDynamic(this,&AMyUnit::HandleRotationAlpha);
		UnitMovement.AddInterpFloat(RotationCurve,tmp);
		UnitMovement.SetLooping(false);
		UnitMovement.SetTimelineLengthMode(TL_LastKeyFrame);
		// FOnTimelineEvent finish;
		// finish.BindDynamic(this,&AMyUnit::FinishRotationAlpha);
		// UnitMovement.SetTimelineFinishedFunc(finish);
	}
	if(JumpCurve)
	{
		FOnTimelineFloat tmp;
		tmp.BindDynamic(this,&AMyUnit::HandleJumpAlpha);
		UnitMovement.AddInterpFloat(JumpCurve,tmp);
		UnitMovement.SetLooping(false);
		UnitMovement.SetTimelineLengthMode(TL_LastKeyFrame);
	}
	// FTimerHandle handle;
	// GetWorld()->GetTimerManager().SetTimer(handle,this,&AMyUnit::testloop,3.0f,true);
}

void AMyUnit::UpdateHoveredAndSelected()
{
	FVector color;
	color  = IsSelected ? FVector(1.0f,0.91f,0.251f) : FVector(1.0f,1.0f,1.0f);
	color *= IsHovered ? 2.5f : 1.0f;
	// color *= IsSelected ? 2.5f : 1.0f;
	MySkeletalMeshComponent->SetVectorParameterValueOnMaterials(TEXT("ColorMultiply"),color);
}

// void AMyUnit::testloop()
// {
// 	testAnimIndex += 1;
// 	testAnimIndex %= 6;
// 	auto inst = MySkeletalMeshComponent->GetAnimInstance();
// 	auto myinter = Cast<IIMyUnitAnimation>(inst);
// 	if(inst == nullptr
// 		 && myinter == nullptr)
// 	{
// 		return;
// 	}
// 	myinter->Execute_SetUnitAnimationState(inst,(EUnitAnimation)testAnimIndex);	
// }

void AMyUnit::RefreshUnit(TObjectPtr<AMy_Pawn> Pawn,TObjectPtr<AGrid> grid,const FIntPoint& index)
{
	My_Pawn = Pawn;
	UnitType = My_Pawn == nullptr ? UnitType : My_Pawn->GetCurrentSelectedUnitType();
	MyGrid = grid;
	GridIndex = index;
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

	MyStats = pData->Stats;
	
	SetActorRotation(FRotator(0,360-90,0));

	UpdateHoveredAndSelected();
}

void AMyUnit::SetHovered(bool h)
{
	IsHovered = h;
	UpdateHoveredAndSelected();
}

void AMyUnit::SetSelected(bool h)
{
	IsSelected = h;
	UpdateHoveredAndSelected();
}

bool AMyUnit::UnitCanWalkOnTile(ETileType TileType)
{
	for(auto one : MyStats.ValidTileTypes)
	{
		if(one == TileType)return true;
	}
	return false;
}

float CalculateRotationAngleToTarget(const FVector& CurrentLocation, const FVector& TargetLocation)
{
	// 计算目标位置相对于当前的位置的向量
	FVector Direction = TargetLocation - CurrentLocation;

	// 确保方向向量在XZ平面上
	Direction.Z = 0.0f;

	// 获取单位方向向量
	FVector UnitDirection = Direction.GetSafeNormal();

	// 计算旋转角度
	float AngleRadians = FMath::Atan2(UnitDirection.Y, UnitDirection.X);
	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

	return AngleDegrees;
}

void AMyUnit::SetWalkPath(TArray<FIntPoint> path)
{
	WalkPath = path;
	WalkPathIndex = 1;
	if(path.IsEmpty())return;
	auto pData = MyGrid->GetTileDataByIndex(WalkPath[WalkPathIndex]);
	
	StartRotateAngles = GetActorForwardVector().Rotation();
	// float AngleDegrees = CalculateRotationAngle(GetActorForwardVector(),GetActorLocation(),pData->Transform.GetLocation());
	float AngleDegrees = CalculateRotationAngleToTarget(GetActorLocation(),pData->Transform.GetLocation());
	// UE_LOG(LogTemp,Log,TEXT("cur location %s next location %s"),*GetActorLocation().ToString(),*pData->Transform.GetLocation().ToString());
	//
	// UE_LOG(LogTemp,Log,TEXT("SetWalkPath StartRotateAngles = %f AngleDegrees = %f "),StartRotateAngles.Yaw,AngleDegrees)
	FinishRotateAngles.Yaw = AngleDegrees - 90;
	UnitMovement.PlayFromStart();
}

void AMyUnit::SetWalkableTile(TArray<FIntPoint> walkableTile)
{
	WalkableTiles =  MoveTemp(walkableTile);
}


// Called every frame
void AMyUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UnitMovement.TickTimeline(DeltaTime);
}

void AMyUnit::HandleLocationAlpha(float Value)
{
	// UE_LOG(LogTemp,Log,TEXT("HandleLocationAlpha %f"),Value)
	if(WalkPath.IsEmpty())return;
	if(WalkPath.Num() <= WalkPathIndex)return;
	FIntPoint& next = WalkPath[WalkPathIndex];
	const FTileData* NextData = MyGrid->GetTileDataByIndex(next);
	if(NextData == nullptr)return;

	const FTileData* CurData = MyGrid->GetTileDataByIndex(GridIndex);
	// UE_LOG(LogTemp,Log,TEXT(" cur = %s next = %s"),CurData->Transform.GetLocation().ToString(),NextData->Transform.GetLocation().ToString());
	FVector tmp = FMath::Lerp(CurData->Transform.GetLocation(),NextData->Transform.GetLocation(),Value);
	SetActorLocation(tmp);
}

float CalculateRotationAngle(FVector CurrentForward,FVector InitialDirection,FVector TargetDirection)
{
	FVector DirectionToNext = (TargetDirection - InitialDirection).GetSafeNormal();
	
	float DotProduct = FVector::DotProduct(CurrentForward.GetSafeNormal(), DirectionToNext);
	float AngleRadians = acosf(FMath::Clamp(DotProduct, -1.0f, 1.0f)); // 避免由于浮点误差超出范围
	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

	// FVector Axis(0.f, 0.f, 1.f); // 绕 Z 轴旋转
	// FQuat QuatRotation(Axis, FMath::DegreesToRadians(AngleDegrees));

	// UE_LOG(LogTemp,Log,TEXT("AngleRadians= %f AngleDegrees = %f QuatRotation = %s"),AngleRadians,AngleDegrees,*QuatRotation.ToString())
	
	FVector CrossProd = FVector::CrossProduct(CurrentForward, DirectionToNext);
	
	if (CrossProd.Z < 0) {
		AngleDegrees = -AngleDegrees; // 如果 Z 分量为负，旋转应为顺时针
	}
	
	// UE_LOG(LogTemp,Log,TEXT("SetWalkPath AngleDegrees = %f "),AngleDegrees)
	return AngleDegrees;
}

void AMyUnit::FinishLocationAlpha()
{
	UE_LOG(LogTemp,Log,TEXT("FinishLocationAlpha"))
	MyGrid->AddTileDataUnitByIndex(GridIndex,nullptr);
	My_Pawn->UpdateTIleByIndex(GridIndex,ETileState::Selected);
	
	GridIndex = WalkPath[WalkPathIndex];
	StartRotateAngles = FinishRotateAngles;
	auto pData = MyGrid->GetTileDataByIndex(GridIndex);
	StartHeight = pData->Transform.GetLocation().Z;
	
	// if(My_Pawn)
	// {
	// 	UE_LOG(LogTemp,Log,TEXT("unit on x = %d y = %d"),GridIndex.X,GridIndex.Y)
	// 	My_Pawn->UpdateTIleByIndex(GridIndex,ETileState::Selected);
	// }
	//
	MyGrid->AddTileDataUnitByIndex(GridIndex,this);
	
	StartRotateAngles = FinishRotateAngles;
	WalkPathIndex++;
	
	if(WalkPath.IsEmpty())return;
	if(WalkPath.Num() <= WalkPathIndex)
	{
		auto inst = MySkeletalMeshComponent->GetAnimInstance();
		auto myinter = Cast<IIMyUnitAnimation>(inst);
		if(inst == nullptr&& myinter == nullptr)
		{
			return;
		}
		myinter->Execute_SetUnitAnimationState(inst,EUnitAnimation::Idle);

		for(const FIntPoint& one : WalkPath)
		{
			MyGrid->RemoveStateFromTile(one,ETileState::PathFinding);
		}
		My_Pawn->UpdateTIleByIndex(GridIndex,ETileState::Selected);
		return;
	}
	
	
	FIntPoint& next = WalkPath[WalkPathIndex];
	pData = MyGrid->GetTileDataByIndex(next);
	float AngleDegrees = CalculateRotationAngleToTarget(GetActorLocation(),pData->Transform.GetLocation()); 
	FinishRotateAngles.Yaw = AngleDegrees - 90;

	TargetHeight = pData->Transform.GetLocation().Z;
	UE_LOG(LogTemp,Log,TEXT(" StartHeight = %f TargetHeight = %f "),StartHeight,TargetHeight);
	UnitMovement.PlayFromStart();
}

void AMyUnit::HandleRotationAlpha(float Value)
{
	auto tmp = FMath::Lerp(StartRotateAngles,FinishRotateAngles,Value);
	SetActorRotation(tmp);
}

void AMyUnit::HandleJumpAlpha(float Value)
{
	FVector vec = GetTransform().GetLocation();
	vec.Z = FMath::Lerp(StartHeight,TargetHeight,Value);
	SetActorLocation(vec);
}

// void AMyUnit::FinishRotationAlpha()
// {
// 	UE_LOG(LogTemp,Log,TEXT("FinishRotationAlpha"))
// 	NeedRotate = false;
// }
