// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUnit.h"

#include "Grid.h"
#include "IdleDirection.h"
#include "IMyUnitAnimation.h"
#include "MyGridPathfinding.h"
#include "My_Pawn.h"
#include "My_Utilities.h"
#include "ShadowUnit.h"
#include "Curves/CurveFloat.h"
#include "Components/TimelineComponent.h"
#include "UnitAbility.h"
#include "UnitAbilityAnim.h"
#include "UnitPathComponent.h"


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

	PathComponent = CreateDefaultSubobject<UUnitPathComponent>(TEXT("PathComponent"));
	PathComponent->ParentPtr = this;
	AddOwnedComponent(PathComponent);
}

void AMyUnit::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	// RefreshUnit(nullptr,nullptr,FIntPoint(0,0));
	
	MyChildActor->CreateChildActor([this](AActor* Actor)
	{
		MyShadowUnit = Cast<AShadowUnit>(Actor);	
		MyShadowUnit->RefreshUnit(this,nullptr,nullptr);
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

FIntPoint AMyUnit::GetStandGridIndex() const
{
	if(NeedToMove())
	{
		return GetTempDestinationGridIndex();
	}
	
	return GetGridIndex();
}

void AMyUnit::RefreshUnit(TObjectPtr<AMy_Pawn> Pawn,TObjectPtr<AGrid> grid,const FIntPoint& index)
{
	My_Pawn = Pawn;
	UnitType = My_Pawn == nullptr ? UnitType : My_Pawn->GetCurrentSelectedUnitType();
	MyGrid = grid;
	TempDestinationGridIndex = GridIndex = index;
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
	MyRuntimeProperty.ActionPoint = 0;//MyProperty.ActionPoint;
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
	AtkNum = MyProperty.AtkCount;
	
	// SetActorRotation(FRotator(0,360-90,0));

	
	
	UpdateHoveredAndSelected();

	//スキルの初期化
	for(int i = 0;i < UnitData->Ability.Num();i++)
	{
		auto AbilityData = UnitData->Ability[i];

		auto ActorAnimName = FString::Format(TEXT("Ability{0}"),{i});
		UChildActorComponent* AAbilityActor = NewObject<UChildActorComponent>(this,UChildActorComponent::StaticClass(),FName(ActorAnimName));
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

	// int MaxRange = 0;
	MaxAtkRange = 0;
	for(int i = 0;i < OwnAbilityAnimList.Num();i++)
	{
		if(OwnAbilityAnimList[i]->GetSkillData().Range.Y > MaxAtkRange)
		{
			MaxAtkRange = OwnAbilityAnimList[i]->GetSkillData().Range.Y;
		}
	}
	// MaxAtkRange += MyRuntimeProperty.Move;

	FUnitColorDataAsset* Color = GetUnitDataColor(UnitType);
	if(!Color->Slot1.IsValid())
	{
		auto m1 = Color->Slot1.LoadSynchronous();
		MySkeletalMeshComponent->SetMaterial(0,m1);
	}
	else
	{
		MySkeletalMeshComponent->SetMaterial(0,Color->Slot1.Get());
	}
	
	if(!Color->Slot2.IsValid())
	{
		auto m1 = Color->Slot2.LoadSynchronous();
		MySkeletalMeshComponent->SetMaterial(1,m1);
	}
	else
	{
		MySkeletalMeshComponent->SetMaterial(1,Color->Slot2.Get());
	}
	
	if(!Color->Slot3.IsValid())
	{
		auto m1 = Color->Slot3.LoadSynchronous();
		MySkeletalMeshComponent->SetMaterial(2,m1);
	}
	else
	{
		MySkeletalMeshComponent->SetMaterial(2,Color->Slot3.Get());
	}
	
	if(!Color->Slot4.IsValid())
	{
		auto m1 = Color->Slot4.LoadSynchronous();
		MySkeletalMeshComponent->SetMaterial(3,m1);
	}
	else
	{
		MySkeletalMeshComponent->SetMaterial(3,Color->Slot4.Get());
	}
	
	if(!Color->Slot5.IsValid())
	{
		auto m1 = Color->Slot5.LoadSynchronous();
		MySkeletalMeshComponent->SetMaterial(4,m1);
	}
	else
	{
		MySkeletalMeshComponent->SetMaterial(4,Color->Slot5.Get());
	}
	
	if(MyShadowUnit)MyShadowUnit->RefreshUnit(this,bp,Color);

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
	MoveIndex = WalkPath[0];
	auto TileDataPtr = MyGrid->GetTileDataByIndex(WalkPath[WalkPathIndex]);
	StartRotateAngles = GetActorForwardVector().Rotation();
	float fAngleDegrees = CalculateRotationAngleToTarget(GetActorLocation(),TileDataPtr->Transform.GetLocation());
	FinishRotateAngles.Yaw = fAngleDegrees - 90;
	IIMyUnitAnimation::Execute_SetUnitAnimationState(MyAnimInstance,EUnitAnimation::Walk);
	UnitMovement.PlayFromStart();
	if(TileDataPtr->UnitOnTile != nullptr)
	{
		TileDataPtr->UnitOnTile->DoDodgeAnim(GetGridIndex());
	}
}


void AMyUnit::SetWalkableTile(TArray<FIntPoint> walkableTile)
{
	WalkableTiles.Empty();
	for(int i = 0;i < walkableTile.Num();i++)
	{
		WalkableTiles.Add(walkableTile[i]);	
	}
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

EUnitDirectType AMyUnit::GetUnitDirect()
{
	float ZRotation = 0;
	if(NeedToMove())
	{
		ZRotation = GetShadowUnitRotation().Yaw;
	}
	else
	{
		ZRotation = GetUnitRotation().Yaw;	
	}
	if(ZRotation <= 0.01f)
	{
		ZRotation += 360.0f;
	}
	if(ZRotation >= 359.0f && ZRotation <= 361.0f)
	{
		return EUnitDirectType::RIGHT;
	}
	if(ZRotation >= 89.0f && ZRotation <= 91.0f)
	{
		return EUnitDirectType::BACKWARD;
	}
	if(ZRotation >= 179.0f && ZRotation <= 181.0f)
	{
		return EUnitDirectType::LEFT;
	}
	if(ZRotation >= 269.0f && ZRotation <= 271.0f)
	{
		return EUnitDirectType::FORWARD;
	}
	return EUnitDirectType::INVALID;
}

void AMyUnit::MoveShadowOnTile(const FVector& location)
{
	if(MyShadowUnit != nullptr)
	{
		ShowShadowUnit();
		MyShadowUnit->SetActorLocation(location);
		float ZRotation = GetUnitRotation().Yaw;
		if(ZRotation <= 0.01f)
		{
			ZRotation += 360.0f;
		}
		if(ZRotation >= 359.0f && ZRotation <= 361.0f)
		{
			TurnShadowRight();
		}
		else if(ZRotation >= 89.0f && ZRotation <= 91.0f)
		{
			TurnShadowBack();
		}
		else if(ZRotation >= 179.0f && ZRotation <= 181.0f)
		{
			TurnShadowLeft();
		}
		else if(ZRotation >= 269.0f && ZRotation <= 271.0f)
		{
			TurnShadowForward();
		}
	}
		
}

int AMyUnit::GetSelectableAbilityNum()
{
	int num = 0;
	for(auto one : OwnAbilityAnimList)
	{
		if(one->IsShowOnCmd())num++;
	}
	return num;
}

void AMyUnit::SetChosenAbility(int ChosenIndex)
{
	ChosenAbilityIndex = FMathf::Clamp(ChosenIndex,0,OwnAbilityAnimList.Num()-1);
}

TObjectPtr<AUnitAbilityAnim> AMyUnit::GetCooperationAbilityAnim()
{
	for(auto one : OwnAbilityAnimList)
	{
		if(one->IsCooperate())return one;
	}
	return nullptr;
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
	AtkNum = MyProperty.AtkCount;
	WalkNum = 1;
	AbilityTargetGridIndex = FIntPoint(-999,-999);
	TempDestinationGridIndex = GridIndex;
	MyRuntimeProperty.ActionPoint += 1;
	MyRuntimeProperty.ActionPoint = FMathf::Clamp(MyRuntimeProperty.ActionPoint,0,3);
}

void AMyUnit::FinishTurn(bool bAsync)
{
	CurrentDistanceToAction = 0;
	//提前计算 单位的攻击范围 因为当前单位已经不会在移动了所以 攻击范围就是固定的
	//このオブジェクトも移動できないんです、だから　攻撃の範囲は決めつけてて非同期で計算しておきます
	if(bAsync)
	{
		FUnitWalkRangeCompleted xxx;
		PathComponent->UnitWalkablePathAsync(0,xxx);	
	}
	else
	{
		PathComponent->UnitWalkablePath(0);
	}
}

bool AMyUnit::HasEnoughAP(int AP)const
{
	return MyRuntimeProperty.ActionPoint >= AP;
}

bool AMyUnit::ConsumeAP(int AP)
{
	if(MyRuntimeProperty.ActionPoint >= AP)
	{
		MyRuntimeProperty.ActionPoint -= AP;
		return true;
	}
	return false;
}

int AMyUnit::AP() const
{
	return MyRuntimeProperty.ActionPoint;
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


void AMyUnit::AddHP(int HP)
{
	MyRuntimeProperty.HP -= HP;
	MyRuntimeProperty.HP = FMathf::Clamp(MyRuntimeProperty.HP,0,MyProperty.HP);
}

FRotator AMyUnit::GetUnitForward() const
{
	if(NeedToMove())
	{
		return MyShadowUnit->GetActorRotation();
	}
	return MySkeletalMeshComponent->GetRelativeRotation();
}

int AMyUnit::GetMaxAtkDeviation() const
{
	int MaxAtkDeviation = 0;
	for (const auto Element : OwnAbilityAnimList)
	{
		MaxAtkDeviation = FMathf::Max(Element->GetSkillData().AllowableDeviation,MaxAtkDeviation);
	}
	return MaxAtkDeviation;
}


void AMyUnit::TurnToTarget(AMyUnit* TargetUnit)
{
	if(TargetUnit == nullptr)return;
	FIntPoint TargetIndex = TargetUnit->GetGridIndex();
	FIntPoint Delta = TargetIndex - GetGridIndex();
	if(Delta.X == 0 )
	{
		if(Delta.Y > 0)
		{//右边
			TurnRight();
		}
		else
		{//左边
			TurnLeft();
		}
	}
	else if(Delta.Y == 0)
	{
		if(Delta.X > 0)
		{
			TurnForward();
		}
		else
		{
			TurnBack();
		}
	}
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
