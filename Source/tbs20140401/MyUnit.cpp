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
	MySkeletalMeshComponent->SetRelativeRotation(FRotator(0,-90,0));
	
	MyChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("Shadow"));
	MyChildActor->SetChildActorClass(AShadowUnit::StaticClass());
	MyChildActor->SetupAttachment(this->RootComponent);

	MyDirectionActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("Direction"));
	MyDirectionActor->SetupAttachment(this->RootComponent);
	

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
	static ConstructorHelpers::FObjectFinder<UCurveFloat> DoDCurve(TEXT("'/Game/Art/Units/DodgeAlpha.DodgeAlpha'"));
	if (DoDCurve.Succeeded())
	{
		DodgeCurve = DoDCurve.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> DeDCurve(TEXT("'/Game/Art/Units/DeathAlpha.DeathAlpha'"));
	if (DeDCurve.Succeeded())
	{
		DeathCurve = DeDCurve.Object;
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
		MyShadowUnit->RefreshUnit(this,nullptr);
		MyShadowUnit->SetHidden(true);
	});

	MyDirectionActor->CreateChildActor([this](AActor* Actor)
	{
		MyDirection = Cast<AIdleDirection>(Actor);
		MyDirectionActor->SetVisibility(false);
		MyDirection->HideArrow();
		MyDirectionActor->SetRelativeLocation(FVector(0,0,100));
		// MyDirectionActor->SetRelativeRotation(FRotator(0,0,0));
		// MyShadowUnit->SetHidden(true);
	});
}

void AMyUnit::BeginDestroy()
{
	Super::BeginDestroy();
	//todo 清理一些资源
	MyGrid = nullptr;
	My_Pawn = nullptr;
	OwnAbilityActorComponents.Empty();
	OwnAbilityAnimList.Empty();
	WalkPath.Empty();
	WalkableTiles.Empty();

	MySkeletalMeshComponent = nullptr;
	MyChildActor = nullptr;
	MyShadowUnit = nullptr;
	MyDirectionActor = nullptr;
	MyDirection = nullptr;
	
	LocationCurve = nullptr;
	RotationCurve = nullptr;
	JumpCurve = nullptr;
	DodgeCurve = nullptr;
	DeathCurve = nullptr;
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
	if(DeathCurve)
	{
		FOnTimelineFloat DeathTmp;
		DeathTmp.BindDynamic(this,&AMyUnit::HandleDeathAlpha);
		DeathMovement.AddInterpFloat(DeathCurve,DeathTmp);
		DeathMovement.SetLooping(false);
		DeathMovement.SetTimelineLengthMode(TL_LastKeyFrame);
		FOnTimelineEvent DeathFinish;
		DeathFinish.BindDynamic(this,&AMyUnit::FinishDeathAlpha);
		DeathMovement.SetTimelineFinishedFunc(DeathFinish);
	}
	if(DodgeCurve)
	{
		FOnTimelineFloat DodgeUpdate;
		DodgeUpdate.BindDynamic(this,&AMyUnit::HandleDodgeAlpha);
		DodgeMovement.AddInterpFloat(DodgeCurve,DodgeUpdate);
		DodgeMovement.SetLooping(false);
		DodgeMovement.SetTimelineLengthMode(TL_LastKeyFrame);
		FOnTimelineEvent DodgeFinish;
		DodgeFinish.BindDynamic(this,&AMyUnit::FinishDodgeAlpha);
		DodgeMovement.SetTimelineFinishedFunc(DodgeFinish);
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
	
}

void AMyUnit::RefreshUnit(TObjectPtr<AMy_Pawn> Pawn,TObjectPtr<AGrid> grid,const FIntPoint& index)
{
	My_Pawn = Pawn;
	UnitType = My_Pawn == nullptr ? UnitType : My_Pawn->GetCurrentSelectedUnitType();
	MyGrid = grid;
	MoveIndex = GridIndex = index;
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

	UClass* bp;
	if(UnitData->Assets.AnimBP.IsValid())
	{
		bp = UnitData->Assets.AnimBP.Get();
	}
	else
	{
		bp = UnitData->Assets.AnimBP.LoadSynchronous();
	}
	for(auto one : bp->Interfaces)
	{
		UE_LOG(LogTemp,Log,TEXT("bp->Interfaces %s"),*one.Class.GetName());
	}
	MySkeletalMeshComponent->SetAnimInstanceClass(bp);
	
	MyStats = UnitData->Stats;
	MyProperty = UnitData->Property;
	//プロパティをコピー
	MyRuntimeProperty.HP = MyProperty.HP;
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
	MyRuntimeProperty.UnitSide = MyProperty.UnitSide;
	
	// SetActorRotation(FRotator(0,360-90,0));

	if(MyShadowUnit)MyShadowUnit->RefreshUnit(this,bp);
	
	UpdateHoveredAndSelected();

	//スキルの初期化
	for(int i = 0;i < UnitData->Ability.Num();i++)
	{
		
		auto AbilityData = UnitData->Ability[i]; 
		switch (AbilityData.SkillId)
		{
		case 10001:
			{
				// auto Ability = NewObject<UUnitAbility_Idle>(this,TEXT("Idle"));
				// Ability->SetSkillData(AbilityData,this);
				// OwnAbilityList.Add(Ability);
				
				UChildActorComponent* AAbilityActor = NewObject<UChildActorComponent>(this,UChildActorComponent::StaticClass(),TEXT("IdleActorAnim"));
				AAbilityActor->SetupAttachment(RootComponent);
				AAbilityActor->SetChildActorClass(AbilityData.SkillAnim.Get());
				AAbilityActor->RegisterComponent();
				AAbilityActor->CreateChildActor([this,AbilityData](AActor* Actor)
				{
					AUnitAbilityAnim* Ability = Cast<AUnitAbilityAnim>(Actor);
					Ability->SetSkillData(AbilityData,this);
					OwnAbilityAnimList.Add(Ability);
				});
				
				OwnAbilityActorComponents.Add(AAbilityActor);
			}
			break;
		case 10002:
		case 20001:
		case 30001:
			{
				// auto Ability = NewObject<UUnitAbility_NormalAtk>(this,TEXT("NormalAttack"));
				// Ability->SetSkillData(AbilityData,this);
				// OwnAbilityList.Add(Ability);
				
				UChildActorComponent* AbilityActor = NewObject<UChildActorComponent>(this,UChildActorComponent::StaticClass(),TEXT("NormalAttackActorAnim"));
				AbilityActor->SetupAttachment(RootComponent);
				AbilityActor->SetChildActorClass(AbilityData.SkillAnim.Get());
				AbilityActor->RegisterComponent();
				AbilityActor->CreateChildActor([this,AbilityData](AActor* Actor)
				{
					AUnitAbilityAnim* Ability = Cast<AUnitAbilityAnim>(Actor);
					Ability->SetSkillData(AbilityData,this);
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

void AMyUnit::SetWalkPath(TArray<FIntPoint> path,FPathCompleted completed)
{
	WalkPath = path;
	WalkPathIndex = 1;
	if(path.IsEmpty())return;
	auto pData = MyGrid->GetTileDataByIndex(WalkPath[WalkPathIndex]);
	
	StartRotateAngles = MySkeletalMeshComponent->GetRelativeRotation();
	// float AngleDegrees = CalculateRotationAngle(GetActorForwardVector(),GetActorLocation(),pData->Transform.GetLocation());
	float AngleDegrees = CalculateRotationAngleToTarget(GetActorLocation(),pData->Transform.GetLocation());
	// UE_LOG(LogTemp,Log,TEXT("cur location %s next location %s"),*GetActorLocation().ToString(),*pData->Transform.GetLocation().ToString());
	//
	// UE_LOG(LogTemp,Log,TEXT("SetWalkPath StartRotateAngles = %f AngleDegrees = %f "),StartRotateAngles.Yaw,AngleDegrees)
	FinishRotateAngles.Yaw = AngleDegrees - 90;
	PathCompleted = completed;

	//下一个即将行走的格子是否有单位
	if(pData->UnitOnTile != nullptr)
	{
		pData->UnitOnTile->DoDodgeAnim(GetGridIndex());
	}
	IIMyUnitAnimation::Execute_SetUnitAnimationState(MyAnimInstance,EUnitAnimation::Walk);
	UnitMovement.PlayFromStart();
}

void AMyUnit::SaveWalkPath(TArray<FIntPoint> Path)
{
	WalkPath = Path;
}

void AMyUnit::StartWalkPath(FPathCompleted Completed)
{
	if(WalkPath.IsEmpty())return;
	
	PathCompleted = Completed;
	WalkPathIndex = 1;
	
	auto pData = MyGrid->GetTileDataByIndex(WalkPath[WalkPathIndex]);
	StartRotateAngles = GetActorForwardVector().Rotation();
	float AngleDegrees = CalculateRotationAngleToTarget(GetActorLocation(),pData->Transform.GetLocation());
	FinishRotateAngles.Yaw = AngleDegrees - 90;
	IIMyUnitAnimation::Execute_SetUnitAnimationState(MyAnimInstance,EUnitAnimation::Walk);
	UnitMovement.PlayFromStart();
	if(pData->UnitOnTile != nullptr)
	{
		pData->UnitOnTile->DoDodgeAnim(GetGridIndex());
	}
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
	DodgeMovement.TickTimeline(DeltaTime);
	DeathMovement.TickTimeline(DeltaTime);
}

void AMyUnit::HandleLocationAlpha(float Value)
{
	// UE_LOG(LogTemp,Log,TEXT("HandleLocationAlpha %f"),Value)
	if(WalkPath.IsEmpty())return;
	if(WalkPath.Num() <= WalkPathIndex)return;
	FIntPoint& next = WalkPath[WalkPathIndex];
	const FTileData* NextData = MyGrid->GetTileDataByIndex(next);
	if(NextData == nullptr)return;

	const FTileData* CurData = MyGrid->GetTileDataByIndex(MoveIndex);
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
	ChosenAbilityIndex = FMathf::Clamp(ChosenIndex,0,OwnAbilityAnimList.Num()-1);
}

void AMyUnit::TurnLeft()
{
	MySkeletalMeshComponent->SetRelativeRotation(FRotator(0,-180,0));
	// SetActorRotation(FRotator(0,360-90-90,0));
}

void AMyUnit::TurnRight()
{
	MySkeletalMeshComponent->SetRelativeRotation(FRotator(0,0,0));
	// SetActorRotation(FRotator(0,0,0));
}

void AMyUnit::TurnForward()
{
	MySkeletalMeshComponent->SetRelativeRotation(FRotator(0,-90,0));
	// SetActorRotation(FRotator(0,360-90,0));
}

void AMyUnit::TurnBack()
{
	MySkeletalMeshComponent->SetRelativeRotation(FRotator(0,90,0));
	// SetActorRotation(FRotator(0,90,0));
}
//-----------------------------------------------------------------------------------------
void AMyUnit::TurnShadowLeft()
{
	MyShadowUnit->SetActorRotation(FRotator(0,-180,0));
}

void AMyUnit::TurnShadowRight()
{
	MyShadowUnit->SetActorRotation(FRotator(0,0,0));
}

void AMyUnit::TurnShadowForward()
{
	MyShadowUnit->SetActorRotation(FRotator(0,-90,0));
}

void AMyUnit::TurnShadowBack()
{
	MyShadowUnit->SetActorRotation(FRotator(0,90,0));
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
	if(NeedToMove())
	{
		FVector ShadowLocation = MyShadowUnit->GetActorLocation();
		ShadowLocation.Z += 100;
		MyDirection->SetActorLocation(ShadowLocation);	
	}
	else
	{
		MyDirection->SetActorLocation(GetActorLocation());
	}
	
	MyDirectionActor->SetVisibility(true);
	MyDirection->ShowArrow();
}

void AMyUnit::HideDirectionArrow()
{
	MyDirection->HideArrow();
	MyDirectionActor->SetVisibility(false);
}

void AMyUnit::BeforeStartTurn()
{
	AtkNum = 1;
	WalkNum = 1;
	AbilityTargetGridIndex = FIntPoint(-999,-999);
	TempDestinationGridIndex = FIntPoint(-999,-999);
}

void AMyUnit::FinishTurn()
{
	CurrentDistanceToAction = 0;
}

void AMyUnit::RotateSelfByDestination(const FIntPoint& StandIndex,const FIntPoint& TargetIndex)
{
	//计算方向 需要旋转角度
	int DeltaX = FMath::Abs(StandIndex.X) - FMath::Abs(TargetIndex.X);
	int DeltaY = FMath::Abs(StandIndex.Y) - FMath::Abs(TargetIndex.Y);
	if(DeltaX == 0)
	{//左右问题
		if(NeedToMove())
		{
			StandIndex.Y < TargetIndex.Y ? TurnShadowRight() : TurnShadowLeft();
		}
		else
		{
			StandIndex.Y < TargetIndex.Y ? TurnRight() : TurnLeft();	
		}
		
	}
	else if(DeltaY == 0)
	{//上下问题
		if(NeedToMove())
		{
			StandIndex.X < TargetIndex.X ? TurnShadowForward() : TurnShadowBack();	
		}
		else
		{
			StandIndex.X < TargetIndex.X ? TurnForward() : TurnBack();
		}
		
	}
	else
	{
		if(DeltaX > DeltaY)
		{//上下问题
			if(NeedToMove())
			{
				StandIndex.X < TargetIndex.X ? TurnShadowForward() : TurnShadowBack();	
			}
			else
			{
				StandIndex.X < TargetIndex.X ? TurnForward() : TurnBack();
			}
			
		}
		else
		{//左右问题
			if(NeedToMove())
			{
				StandIndex.Y < TargetIndex.Y ? TurnShadowRight() : TurnShadowLeft();	
			}
			else
			{
				StandIndex.Y < TargetIndex.Y ? TurnRight() : TurnLeft();	
			}
		}
	}
}

void AMyUnit::DoDodgeAnim(const FIntPoint& FromIndex)
{
	DodgeIndex = GridIndex;
	DodgeIndex = GridIndex;
	int DeltaX = GridIndex.X - FromIndex.X;
	int DeltaY = GridIndex.Y - FromIndex.Y;
	bool IsUpDown = FMath::RandBool();
	if(DeltaX == 0)
	{
		if(IsUpDown)DodgeIndex.X += 1;
		else DodgeIndex.X -= 1;
		
	}
	else if(DeltaY == 0)
	{
		if(IsUpDown)DodgeIndex.Y += 1;
		else DodgeIndex.Y -= 1;
	}
	else if(DeltaX > 0)
	{
		if(IsUpDown)DodgeIndex.X += 1;
		else DodgeIndex.X -= 1;
	}
	UE_LOG(LogTemp,Log,TEXT("DodgeMovement.PlayFromStart()"))
	DodgeMovement.PlayFromStart();
}

void AMyUnit::DoDeadAnim(FDeathCompleted Completed)
{
	DeathCompleted = Completed;
	DeathMovement.PlayFromStart();
}


FRotator AMyUnit::GetUnitForward() const
{
	if(NeedToMove())
	{
		return MyShadowUnit->GetActorRotation();
	}
	return MySkeletalMeshComponent->GetRelativeRotation();
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

	MoveIndex = WalkPath[WalkPathIndex];
	StartRotateAngles = FinishRotateAngles;
	auto pData = MyGrid->GetTileDataByIndex(MoveIndex);
	StartHeight = pData->Transform.GetLocation().Z;
	
	StartRotateAngles = FinishRotateAngles;
	WalkPathIndex++;
	
	if(WalkPath.IsEmpty())return;
	if(WalkPath.Num() <= WalkPathIndex)
	{
		IIMyUnitAnimation::Execute_SetUnitAnimationState(MyAnimInstance,EUnitAnimation::Idle);

		for(const FIntPoint& one : WalkPath)
		{
			MyGrid->RemoveStateFromTile(one,ETileState::PathFinding);
		}
		MyGrid->AddTileDataUnitByIndex(GridIndex,nullptr);
		TempDestinationGridIndex = GridIndex = MoveIndex;
		MyGrid->AddTileDataUnitByIndex(GridIndex,this);
		MyShadowUnit->SetActorLocation(GetActorLocation());
		PathCompleted.Execute();
		PathCompleted.Unbind();
		return;
	}
	
	
	FIntPoint& next = WalkPath[WalkPathIndex];
	pData = MyGrid->GetTileDataByIndex(next);
	float AngleDegrees = CalculateRotationAngleToTarget(GetActorLocation(),pData->Transform.GetLocation()); 
	FinishRotateAngles.Yaw = AngleDegrees - 90;

	TargetHeight = pData->Transform.GetLocation().Z;
	if(pData->UnitOnTile != nullptr)
	{
		pData->UnitOnTile->DoDodgeAnim(MoveIndex);
	}
	// UE_LOG(LogTemp,Log,TEXT(" StartHeight = %f TargetHeight = %f "),StartHeight,TargetHeight);

	UnitMovement.PlayFromStart();
}

void AMyUnit::HandleDeathAlpha(float Value)
{
	if(Value > 0)return;

	UE_LOG(LogTemp,Log,TEXT("AMyUnit::HandleDeathAlpha %f"),Value)
	IIMyUnitAnimation::Execute_SetUnitAnimationState(MyAnimInstance,EUnitAnimation::Death);
}

void AMyUnit::FinishDeathAlpha()
{
	UE_LOG(LogTemp,Log,TEXT("AMyUnit::FinishDeathAlpha"))
	DeathCompleted.Execute();
}

void AMyUnit::HandleDodgeAlpha(float Value)
{
	UE_LOG(LogTemp,Log,TEXT("HandleDodgeAlpha %f"),Value)
	const FTileData* CurrentTile = MyGrid->GetTileDataByIndex(GridIndex);
	const FTileData* DodgeTile = MyGrid->GetTileDataByIndex(DodgeIndex);
	auto tmp = FMath::Lerp(CurrentTile->Transform.GetLocation(),DodgeTile->Transform.GetLocation(),Value);
	SetActorLocation(tmp);
}

void AMyUnit::FinishDodgeAlpha()
{
	UE_LOG(LogTemp,Log,TEXT("FinishDodgeAlpha "))
}

void AMyUnit::HandleRotationAlpha(float Value)
{
	auto tmp = FMath::Lerp(StartRotateAngles,FinishRotateAngles,Value);
	// SetActorRotation(tmp);
	MySkeletalMeshComponent->SetRelativeRotation(tmp);
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
