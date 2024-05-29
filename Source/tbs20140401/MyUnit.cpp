// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUnit.h"

#include "Grid.h"
#include "IdleDirection.h"
#include "IMyUnitAnimation.h"
#include "My_Pawn.h"
#include "My_Utilities.h"
#include "ShadowUnit.h"
#include "Curves/CurveFloat.h"
#include "Components/TimelineComponent.h"
#include "UnitAbility.h"
#include "UnitAbility_NormalAtk.h"
#include "UnitAbility_Idle.h"
#include "UnitAbilityAnim.h"


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

	MyChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("Shadow"));
	MyChildActor->SetChildActorClass(AShadowUnit::StaticClass());
	MyChildActor->SetupAttachment(this->RootComponent);

	MyDirectionActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("Direction"));
	MyDirectionActor->SetupAttachment(this->RootComponent);
	
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

	static ConstructorHelpers::FClassFinder<AActor> ActorBPClass(TEXT("/Game/Art/Units/CPP_IdleDirection"));
	if (ActorBPClass.Class != nullptr)
	{
		MyDirectionActor->SetChildActorClass(ActorBPClass.Class);
	}
}

void AMyUnit::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	// RefreshUnit(nullptr,nullptr,FIntPoint(0,0));
	
	MyChildActor->CreateChildActor([this](AActor* Actor)
	{
		MyShadowUnit = Cast<AShadowUnit>(Actor);	
		MyShadowUnit->RefreshUnit(this);
		MyShadowUnit->SetHidden(true);
	});

	MyDirectionActor->CreateChildActor([this](AActor* Actor)
	{
		MyDirection = Cast<AIdleDirection>(Actor);
		MyDirectionActor->SetVisibility(false);
		MyDirectionActor->SetRelativeLocation(FVector(0,0,100));
		MyDirectionActor->SetRelativeRotation(FRotator(0,90,0));
		// MyShadowUnit->SetHidden(true);
	});
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

void AMyUnit::OnAnimInstanceCompleted()
{
	MyAnimInstance = MySkeletalMeshComponent->GetAnimInstance();
	MyUnitAnimation = Cast<IIMyUnitAnimation>(MyAnimInstance);
}

void AMyUnit::RefreshUnit(TObjectPtr<AMy_Pawn> Pawn,TObjectPtr<AGrid> grid,const FIntPoint& index)
{
	My_Pawn = Pawn;
	UnitType = My_Pawn == nullptr ? UnitType : My_Pawn->GetCurrentSelectedUnitType();
	MyGrid = grid;
	GridIndex = index;
	const FUnitData* UnitData = GetUnitData(UnitType);
	if(UnitData == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("%d data is null"),UnitType);
		return;
	}

	if(UnitData->Assets.SkeletalMesh.IsValid())
	{
		MySkeletalMeshComponent->SetSkeletalMesh(UnitData->Assets.SkeletalMesh.Get());
		// ShadowSkeletalMeshComponent->SetSkeletalMesh(pData->Assets.SkeletalMesh.Get());
	}
	else
	{
		auto skeletalMesh = UnitData->Assets.SkeletalMesh.LoadSynchronous();
		MySkeletalMeshComponent->SetSkeletalMesh(skeletalMesh);
		// ShadowSkeletalMeshComponent->SetSkeletalMesh(skeletalMesh);
	}
	
	FOnAnimInitialized Initialized;
	Initialized.AddDynamic(this,&AMyUnit::OnAnimInstanceCompleted);
	MySkeletalMeshComponent->OnAnimInitialized = Initialized;

	if(UnitData->Assets.AnimBP.IsValid())
	{
		MySkeletalMeshComponent->SetAnimInstanceClass(UnitData->Assets.AnimBP.Get());
	}
	else
	{
		auto bp = UnitData->Assets.AnimBP.LoadSynchronous();
		MySkeletalMeshComponent->SetAnimInstanceClass(bp);
	}
	
	MyStats = UnitData->Stats;
	MyProperty = UnitData->Property;
	//プロパティをコピー
	MyRuntimeProperty.Move = MyProperty.Move;
	MyRuntimeProperty.ActionPoint = MyProperty.ActionPoint;
	MyRuntimeProperty.Power = MyProperty.Power;
	MyRuntimeProperty.PhysicDefend = MyProperty.PhysicDefend;
	MyRuntimeProperty.MagicPower = MyProperty.MagicPower;
	MyRuntimeProperty.MagicDefend = MyProperty.MagicDefend;
	MyRuntimeProperty.Lucky = MyProperty.Lucky;
	MyRuntimeProperty.HitProb = MyProperty.HitProb;
	MyRuntimeProperty.Speed = MyProperty.Speed;
	MyRuntimeProperty.Dodge = MyProperty.Dodge;
	MyRuntimeProperty.Jump = MyProperty.Jump;
	MyRuntimeProperty.FireResistance = MyProperty.FireResistance;
	MyRuntimeProperty.IceResistance = MyProperty.IceResistance;
	MyRuntimeProperty.WindResistance = MyProperty.WindResistance;
	MyRuntimeProperty.ThunderResistance = MyProperty.ThunderResistance;
	
	SetActorRotation(FRotator(0,360-90,0));

	if(MyShadowUnit)MyShadowUnit->RefreshUnit(this);
	
	UpdateHoveredAndSelected();

	//スキルの初期化
	for(int i = 0;i < UnitData->Ability.Num();i++)
	{
		
		auto AbilityData = UnitData->Ability[i]; 
		switch (AbilityData.SkillId)
		{
		case 10001:
			{
				auto Ability = NewObject<UUnitAbility_Idle>(this,TEXT("Idle"));
				Ability->SetSkillData(AbilityData,this);
				OwnAbilityList.Add(Ability);
				
				UChildActorComponent* AAbilityActor = NewObject<UChildActorComponent>(this,UChildActorComponent::StaticClass(),TEXT("IdleActorAnim"));
				AAbilityActor->SetupAttachment(RootComponent);
				AAbilityActor->SetChildActorClass(AbilityData.SkillAnim.Get());
				AAbilityActor->RegisterComponent();
				AAbilityActor->CreateChildActor([this](AActor* Actor)
				{
					AUnitAbilityAnim* Ability = Cast<AUnitAbilityAnim>(Actor);
					OwnAbilityAnimList.Add(Ability);
				});
				
				OwnAbilityActorComponents.Add(AAbilityActor);
			}
			break;
		case 10002:
		case 20001:
		case 30001:
			{
				auto Ability = NewObject<UUnitAbility_NormalAtk>(this,TEXT("NormalAttack"));
				Ability->SetSkillData(AbilityData,this);
				OwnAbilityList.Add(Ability);
				
				UChildActorComponent* AbilityActor = NewObject<UChildActorComponent>(this,UChildActorComponent::StaticClass(),TEXT("NormalAttackActorAnim"));
				AbilityActor->SetupAttachment(RootComponent);
				AbilityActor->SetChildActorClass(AbilityData.SkillAnim.Get());
				AbilityActor->RegisterComponent();
				AbilityActor->CreateChildActor([this](AActor* Actor)
				{
					AUnitAbilityAnim* Ability = Cast<AUnitAbilityAnim>(Actor);
					OwnAbilityAnimList.Add(Ability);
				});
				OwnAbilityActorComponents.Add(AbilityActor);
			}
			break;
		default:
			{
				UE_LOG(LogTemp,Log,TEXT("error skillid %d"),UnitData->Ability[i].SkillId)
				// auto OneAbility = NewObject<UUnitAbility>(this);
				// OwnAbilityList.Add(OneAbility);	
			}
			break;
		}
	}
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

bool AMyUnit::IsInWalkableRange(const FIntPoint& index)
{
	if(WalkableTiles.IsEmpty())return false;
	
	return WalkableTiles.Contains(index);
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

void AMyUnit::ShowShadowUnit()
{
	if(MyShadowUnit)
		MyShadowUnit->SetActorHiddenInGame(false);
}

void AMyUnit::HideShadowUnit()
{
	if(MyShadowUnit)
		MyShadowUnit->SetActorHiddenInGame(true);
}

void AMyUnit::MoveShadowOnTile(const FVector& location)
{
	if(MyShadowUnit != nullptr)
	{
		ShowShadowUnit();
		MyShadowUnit->SetActorLocation(location);
	}
		
}

void AMyUnit::SetChosenAbility(int ChosenIndex)
{
	ChosenAbilityIndex = FMathf::Clamp(ChosenIndex,0,OwnAbilityList.Num()-1);
}

void AMyUnit::TurnLeft()
{
	MySkeletalMeshComponent->SetRelativeRotation(FRotator(0,-90,0));
	// SetActorRotation(FRotator(0,360-90-90,0));
}

void AMyUnit::TurnRight()
{
	MySkeletalMeshComponent->SetRelativeRotation(FRotator(0,90,0));
	// SetActorRotation(FRotator(0,0,0));
}

void AMyUnit::TurnForward()
{
	MySkeletalMeshComponent->SetRelativeRotation(FRotator(0,0,0));
	// SetActorRotation(FRotator(0,360-90,0));
}

void AMyUnit::TurnBack()
{
	MySkeletalMeshComponent->SetRelativeRotation(FRotator(0,180,0));
	// SetActorRotation(FRotator(0,90,0));
}

void AMyUnit::ShowDirectionArrow()
{
	const auto& CurrentRotator = MySkeletalMeshComponent->GetRelativeRotation();
	if(FMathf::Ceil(CurrentRotator.Yaw) == 0)
	{
		MyDirection->DoUpArrowAnimation();
	}
	else if(FMathf::Ceil(CurrentRotator.Yaw) == 180)
	{
		MyDirection->DoDownArrowAnimation();
	}
	else if(FMathf::Ceil(CurrentRotator.Yaw) == -90)
	{
		MyDirection->DoLeftArrowAnimation();
	}
	else if(FMathf::Ceil(CurrentRotator.Yaw) == 90)
	{
		MyDirection->DoRightArrowAnimation();
	}
	MyDirectionActor->SetVisibility(true);
}

void AMyUnit::HideDirectionArrow()
{
	MyDirectionActor->SetVisibility(false);
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
	My_Pawn->UpdateTileStatusByIndex(GridIndex,ETileState::Selected);
	
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
		if(MyUnitAnimation == nullptr)
		{
			return;
		}
		MyUnitAnimation->Execute_SetUnitAnimationState(MyAnimInstance,EUnitAnimation::Idle);

		for(const FIntPoint& one : WalkPath)
		{
			MyGrid->RemoveStateFromTile(one,ETileState::PathFinding);
		}
		My_Pawn->UpdateTileStatusByIndex(GridIndex,ETileState::Selected);
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
