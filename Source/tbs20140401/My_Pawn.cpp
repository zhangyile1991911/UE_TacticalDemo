// Fill out your copyright notice in the Description page of Project Settings.


#include "My_Pawn.h"

#include "Action_AddTile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EventCenter.h"
#include "Grid.h"
#include "MyAction.h"
#include "MyCombatSystem.h"
#include "MyGridPathfinding.h"
#include "MyUnit.h"
#include "PawnProcess.h"
#include "PawnProcess_CMD.h"
#include "UPawnProcess_Normal.h"
#include "Kismet/GameplayStatics.h"
#include "MyHUD.h"
#include "PathPointInst.h"
#include "PawnProcess_BeforeTurn.h"
#include "PawnProcess_CalcAnim.h"
#include "PawnProcess_Idle.h"
#include "PawnProcess_ChooseTarget.h"
#include "PawnProcess_Move.h"


// Sets default values
AMy_Pawn::AMy_Pawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if(this->RootComponent == nullptr)
	{
		// UE_LOG(LogTemp,Log,TEXT("this->RootComponent == nullptr"))
		this->RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
	}

	m_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	m_springArm->SetupAttachment(this->RootComponent);
	m_springArm->bDoCollisionTest = false;
	
	m_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_camera->SetupAttachment(m_springArm);

	
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAsset(TEXT("InputAction'/Game/Input/Actions/zoom'"));
	if (InputActionAsset.Succeeded())
	{
		MouseZoomAction = InputActionAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRotAsset(TEXT("InputAction'/Game/Input/Actions/CamRot'"));
	if (InputActionRotAsset.Succeeded())
	{
		CamRotAction = InputActionRotAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMovAsset(TEXT("InputAction'/Game/Input/Actions/CamMov'"));
	if (InputActionMovAsset.Succeeded())
	{
		CamMoveAction = InputActionMovAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLeftAsset(TEXT("InputAction'/Game/Input/Actions/MouseLeftClick'"));
	if (InputActionLeftAsset.Succeeded())
	{
		MouseLeftClickAction = InputActionLeftAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRightAsset(TEXT("InputAction'/Game/Input/Actions/MouseRightClick'"));
	if (InputActionRightAsset.Succeeded())
	{
		MouseRightClickAction = InputActionRightAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputUnitMoveAsset(TEXT("InputAction'/Game/Input/Actions/UnitMove'"));
    if (InputUnitMoveAsset.Succeeded())
    {
    	UnitMoveAction = InputUnitMoveAsset.Object;
    }

	static ConstructorHelpers::FObjectFinder<UInputAction> ConfirmAsset(TEXT("InputAction'/Game/Input/Actions/ConfirmAct'"));
	if (ConfirmAsset.Succeeded())
	{
		ConfirmAction = ConfirmAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> CancelAsset(TEXT("InputAction'/Game/Input/Actions/CancelAct'"));
	if (CancelAsset.Succeeded())
	{
		CancelAction = CancelAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> SpaceAsset(TEXT("InputAction'/Game/Input/Actions/SpaceAct'"));
	if (SpaceAsset.Succeeded())
	{
		SpaceAction = SpaceAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TabAsset(TEXT("InputAction'/Game/Input/Actions/TabAct'"));
	if (TabAsset.Succeeded())
	{
		TabAction = TabAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> KeyboardLeftAsset(TEXT("InputAction'/Game/Input/Actions/LeftAct'"));
	if (KeyboardLeftAsset.Succeeded())
	{
		KeyBoardLeftAction = KeyboardLeftAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> KeyboardRightAsset(TEXT("InputAction'/Game/Input/Actions/RightAct'"));
	if (KeyboardRightAsset.Succeeded())
	{
		KeyBoardRightAction = KeyboardRightAsset.Object;
	}
	
}

// Called when the game starts or when spawned
void AMy_Pawn::BeginPlay()
{
	Super::BeginPlay();
	// UE_LOG(LogTemp,Log,TEXT("AMy_Pawn::BeginPlay()"))

	m_curArmLength = 3000.0f;
	m_springArm->TargetArmLength = m_curArmLength;
	m_springArm->SetWorldRotation(FRotator(-60.0f,0.0,0));

	m_locationDesired = GetActorLocation();
	m_rotationDesired = GetActorRotation();

	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(),AGrid::StaticClass());
	MyGrid = Cast<AGrid>(actor);

	actor = UGameplayStatics::GetActorOfClass(GetWorld(),AMyCombatSystem::StaticClass());
	MyCombatSystem = Cast<AMyCombatSystem>(actor);

	actor = UGameplayStatics::GetActorOfClass(GetWorld(),AMyGridPathfinding::StaticClass());
	MyGridPathfinding = Cast<AMyGridPathfinding>(actor);

	actor = UGameplayStatics::GetActorOfClass(GetWorld(),APathPointInst::StaticClass());
	MyPathPointInst = Cast<APathPointInst>(actor);
	// OnTileTYpeChanged.AddDynamic(this,&AMy_Pawn::SetCurrentTileType);

	NormalProcess = NewObject<UUPawnProcess_Normal>(this);
	CmdProcess = NewObject<UPawnProcess_CMD>(this);
	BeforeTurnProcess = NewObject<UPawnProcess_BeforeTurn>(this);
	IdleProcess = NewObject<UPawnProcess_Idle>(this);
	ChooseTargetProcess = NewObject<UPawnProcess_ChooseTarget>(this);
	MoveProcess = NewObject<UPawnProcess_Move>(this);
	CalcAnimProcess = NewObject<UPawnProcess_CalcAnim>(this);

	EventCenter = NewObject<UEventCenter>(this);
	
	CurrentProcess = nullptr;

	// 获取当前的玩家控制器
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{// 获取当前的 HUD 实例
		AHUD* HUD = PlayerController->GetHUD();
		if (HUD)
		{
			// 检查 HUD 是否是你的自定义 HUD 类
			MyHUDInstance = Cast<AMyHUD>(HUD);
			
		}
	}
	
}

bool FVectorIsNearly(const FVector& V1,const FVector& V2,float ErrorTolerance)
{
	bool bX = FMath::IsNearlyEqual(V1.X,V2.X,ErrorTolerance);

	bool bY = FMath::IsNearlyEqual(V1.Y,V2.Y,ErrorTolerance);

	bool bZ = FMath::IsNearlyEqual(V1.Y,V2.Y,ErrorTolerance);
	return bX&&bY&&bZ;
}
// Called every frame
void AMy_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!FMath::IsNearlyEqual(m_curArmLength,m_springArm->TargetArmLength,0.05f))
	{
		m_springArm->TargetArmLength = FMath::FInterpTo(m_springArm->TargetArmLength,m_curArmLength,DeltaTime,Zoom_Interp);	
		OnCameraActing.Broadcast();
	}

	const auto ActorLocation = GetActorLocation();
	if(!FVectorIsNearly(ActorLocation,m_locationDesired,0.05f))
	{
		SetActorLocation(FMath::VInterpTo(ActorLocation,m_locationDesired,DeltaTime,Location_Interp));
		OnCameraActing.Broadcast();
	}
	

	const auto ActorRotation = GetActorRotation().Yaw;
	if(!FMath::IsNearlyEqual(ActorRotation,m_rotationDesired.Yaw,0.05f))
	{
		SetActorRotation(FMath::RInterpTo(GetActorRotation(),m_rotationDesired,DeltaTime,Rotation_Interp));
		OnCameraActing.Broadcast();
	}
	

	if(!IsStartGame)UpdateTileUnderCursor();
	if(CurrentProcess)CurrentProcess->TickProcess();
}

// Called to bind functionality to input
void AMy_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// UE_LOG(LogTemp,Log,TEXT("AMy_Pawn::SetupPlayerInputComponent"))
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// UE_LOG(LogTemp,Log,TEXT("BindActiion MouseZoomAction"))
		// EnhancedInputComponent->BindAction(MouseZoomAction,ETriggerEvent::Ongoing,this,&AMy_Pawn::MouseZooming);
		EnhancedInputComponent->BindAction(MouseZoomAction,ETriggerEvent::Triggered,this,&AMy_Pawn::MouseZooming);
		EnhancedInputComponent->BindAction(CamRotAction,ETriggerEvent::Triggered,this,&AMy_Pawn::CamClockWise);
		EnhancedInputComponent->BindAction(CamMoveAction,ETriggerEvent::Triggered,this,&AMy_Pawn::CamMove);
		EnhancedInputComponent->BindAction(MouseLeftClickAction,ETriggerEvent::Triggered,this,&AMy_Pawn::MouseLeftClick);
		EnhancedInputComponent->BindAction(MouseRightClickAction,ETriggerEvent::Triggered,this,&AMy_Pawn::MouseRightClick);
		EnhancedInputComponent->BindAction(UnitMoveAction,ETriggerEvent::Triggered,this,&AMy_Pawn::UnitMove);
		EnhancedInputComponent->BindAction(ConfirmAction,ETriggerEvent::Completed,this,&AMy_Pawn::ConfirmClick);
		EnhancedInputComponent->BindAction(CancelAction,ETriggerEvent::Completed,this,&AMy_Pawn::CancelClick);
		EnhancedInputComponent->BindAction(SpaceAction,ETriggerEvent::Completed,this,&AMy_Pawn::SpaceClick);
		EnhancedInputComponent->BindAction(TabAction,ETriggerEvent::Completed,this,&AMy_Pawn::TabClick);
		EnhancedInputComponent->BindAction(KeyBoardLeftAction,ETriggerEvent::Completed,this,&AMy_Pawn::LeftClick);
		EnhancedInputComponent->BindAction(KeyBoardRightAction,ETriggerEvent::Completed,this,&AMy_Pawn::RightClick);
	}
}

void AMy_Pawn::UpdateTileUnderCursor()
{
	auto playerIndex = GetWorld()->GetFirstPlayerController();
	FIntPoint index = MyGrid->GetTileIndexUnderCursor(playerIndex,true,CanHoverEmptySpace());
	if(HoveredTile != index)
	{
		MyGrid->RemoveStateFromTile(HoveredTile,ETileState::Hovered);
		MyGrid->AddStateToTile(index,ETileState::Hovered);
		
		HoveredTile = index;
	}

	if(HoveredUnit)
	{
		HoveredUnit->SetHovered(false);
	}
	
	auto pData = MyGrid->GetTileDataByIndex(index);
	if(pData == nullptr)return;

	if(pData->UnitOnTile == nullptr)return;

	pData->UnitOnTile->SetHovered(true);
	HoveredUnit = pData->UnitOnTile;
	
}

void AMy_Pawn::UpdateTileStatusByIndex(const FIntPoint& index, ETileState state)
{
	MyGrid->AddStateToTile(index,state);
	if(state == ETileState::Selected)
	{
		if(index != SelectedTile)MyGrid->RemoveStateFromTile(SelectedTile,state);
		SelectedTile = index;
	}
	const auto TileDataPtr = MyGrid->GetTileDataByIndex(index);

	if(SelectedUnit)SelectedUnit->SetSelected(false);
	if(TileDataPtr == nullptr)return;
	if(TileDataPtr->UnitOnTile == nullptr)return;

	TileDataPtr->UnitOnTile->SetSelected(true);
	SelectedUnit = TileDataPtr->UnitOnTile;
}

void AMy_Pawn::RemoveTileStateByIndex(const FIntPoint& index, ETileState state)
{
	MyGrid->RemoveStateFromTile(index,state);
}

void AMy_Pawn::SetSelectedActions(UClass* left, UClass* right)
{
	if(MouseLeftAction != nullptr)
	{
		MouseLeftAction->Destroy();
		MouseLeftAction = nullptr;
	}

	if(MouseRightAction != nullptr)
	{
		MouseRightAction->Destroy();
		MouseRightAction = nullptr;
	}
	
	auto world = GetWorld();
	if(left)MouseLeftAction = Cast<AMyAction>(world->SpawnActor(left));
	if(right)MouseRightAction = Cast<AMyAction>(world->SpawnActor(right));
}

void AMy_Pawn::AddNewTileUnderCursor()
{
	FIntPoint index = MyGrid->GetTileIndexUnderCursor(GetWorld()->GetFirstPlayerController(),true,CanHoverEmptySpace());
	MyGrid->AddNewOneTIle(index);
}

// void AMy_Pawn::RemoveTileUnderCursor()
// {
// }

void AMy_Pawn::RemoveTileUnderCursor(const FIntPoint& index)
{
	MyGrid->RemoveOneTIle(index);
}

void AMy_Pawn::MouseZooming(const FInputActionValue& val)
{
	float tmp = val.Get<FInputActionValue::Axis1D>();
	if(IsStartGame)
	{
		if(CurrentProcess)CurrentProcess->HandleZooming(tmp);
	}
	else
	{
		CameraControlZooming(tmp);
	}
	// UE_LOG(LogTemp,Log,TEXT("AMy_Pawn::MouseZooming %f"),tmp)
	// tmp *= Zoom_Speed;
	// tmp += m_curArmLength;
	// m_curArmLength = FMathf::Clamp(tmp,MaxMin_ArmLength.X,MaxMin_ArmLength.Y);
	//需要有个通知 UI去更新自己位置
	
}

void AMy_Pawn::CamClockWise(const FInputActionValue& val)
{
	// UE_LOG(LogTemp,Log,TEXT("AMy_Pawn::CamClockWise %f "),m_rotationDesired.Yaw)
	float b1 = val.Get<FInputActionValue::Axis1D>();
	// UE_LOG(LogTemp,Log,TEXT("AMy_Pawn::CamClockWise %f"),b1)
	if(b1 > 0.0f)
	{
		if(IsStartGame)
		{
			if(CurrentProcess)CurrentProcess->HandleLeftInput();
		}
		else
		{
			CameraControlLeft();
		}
	}
	else
	{
		if(IsStartGame)
		{
			if(CurrentProcess)CurrentProcess->HandleRightInput();	
		}
		else
		{
			CameraControlRight();
		}
	}

	float Yaw = m_rotationDesired.Yaw;
	if(Yaw < 0)
	{
		Yaw = static_cast<int>(m_rotationDesired.Yaw)%360 + 360.0f;	
	}
	if(Yaw > 360)
	{
		Yaw = static_cast<int>(m_rotationDesired.Yaw)%360;
	}
	
	// UE_LOG(LogTemp,Log,TEXT("AMy_Pawn::CamClockWise static_cast<int> %f "),Yaw)
	if(FMath::IsNearlyEqual(Yaw,270,1.0f))
	{
		CameraDirect = ECameraDirectType::RIGHT;
		// UE_LOG(LogTemp,Log,TEXT("ECameraDirectType::RIGHT %f"),Yaw)
	}
	else if(FMath::IsNearlyEqual(Yaw,180,1.0f))
	{
		CameraDirect = ECameraDirectType::BACKWARD;
		// UE_LOG(LogTemp,Log,TEXT("ECameraDirectType::BACKWARD %f"),Yaw)
	}
	else if(FMath::IsNearlyEqual(Yaw,90,1.0f))
	{
		CameraDirect = ECameraDirectType::LEFT;
		// UE_LOG(LogTemp,Log,TEXT("ECameraDirectType::LEFT %f"),Yaw)
	}
	else if(FMath::IsNearlyEqual(Yaw,360,1.0f) || FMath::IsNearlyEqual(Yaw,0,1.0f))
	{
		CameraDirect = ECameraDirectType::FORWARD;
		// UE_LOG(LogTemp,Log,TEXT("ECameraDirectType::FORWARD %f"),Yaw)
	}
	//需要有个通知 UI去更新自己位置
}


void AMy_Pawn::CamMove(const FInputActionValue& val)
{
	if(IsStartGame)return;
	FVector2D tmp = val.Get<FInputActionValue::Axis2D>();
	//UE_LOG(LogTemp,Log,TEXT("AMy_Pawn::CamMove Location_Speed = %f"),Location_Speed);
	float forward = tmp.Y * Location_Speed;
	float horizontal = tmp.X * Location_Speed;

	FVector actorForward = GetActorForwardVector();
	actorForward *= forward;
	FVector actorRightForward = GetActorRightVector();
	actorRightForward *= horizontal;
	m_locationDesired = m_locationDesired + actorForward + actorRightForward;
	// m_locationDesired.X += forward;
	// m_locationDesired.Y += horizontal;

}

void AMy_Pawn::MouseLeftClick(const FInputActionValue& value)
{
	UE_LOG(LogTemp,Log,TEXT("AMy_Pawn::MouseLeftClick"))
	if(MouseLeftAction)
	{
		FIntPoint index = MyGrid->GetTileIndexUnderCursor(GetWorld()->GetFirstPlayerController(),true,CanHoverEmptySpace());
		MouseLeftAction->ExecuteAction(index);	
	}
	
}

void AMy_Pawn::MouseRightClick(const FInputActionValue& value)
{
	UE_LOG(LogTemp,Log,TEXT("AMy_Pawn::MouseRightClick"))
	if(MouseRightAction)
	{
		FIntPoint index = MyGrid->GetTileIndexUnderCursor(GetWorld()->GetFirstPlayerController(),true,CanHoverEmptySpace());
		MouseRightAction->ExecuteAction(index);	
	}
}

void AMy_Pawn::UnitMove(const FInputActionValue& value)
{
	if(!IsStartGame)return;
	// UE_LOG(LogTemp,Log,TEXT("AMy_Pawn::UnitMove"))
	FVector2D tmp = value.Get<FInputActionValue::Axis2D>();
	// UE_LOG(LogTemp,Log,TEXT("AMy_Pawn::UnitMove %f %f "),tmp.X,tmp.Y)
	
	CurrentProcess->HandleDirectionInput(tmp);
}

void AMy_Pawn::ConfirmClick(const FInputActionValue& value)
{
	if(!IsStartGame)return;
	CurrentProcess->HandleConfirmInput();
}

void AMy_Pawn::CancelClick(const FInputActionValue& value)
{
	if(!IsStartGame)return;
	CurrentProcess->HandleCancelInput();
}

void AMy_Pawn::SpaceClick(const FInputActionValue& value)
{
	if(!IsStartGame)return;
	CurrentProcess->HandleSpaceInput();
}

void AMy_Pawn::TabClick(const FInputActionValue& value)
{
	if(!IsStartGame)return;
	CurrentProcess->HandleTabInput();
}

void AMy_Pawn::LeftClick(const FInputActionValue& value)
{
	if(IsStartGame)return;
	CurrentProcess->HandleLeftInput();
}

void AMy_Pawn::RightClick(const FInputActionValue& value)
{
	if(IsStartGame)return;
	CurrentProcess->HandleRightInput();
}

void AMy_Pawn::SwitchProcess(TObjectPtr<UPawnProcess> NextProcess)
{
	if(CurrentProcess != nullptr)
	{
		CurrentProcess->ExitProcess();
	}
	CurrentProcess = NextProcess;
	CurrentProcess->EnterProcess(this);
}

bool AMy_Pawn::CanHoverEmptySpace()
{
	UClass* add = AAction_AddTile::StaticClass();
	if(MouseLeftAction == nullptr)return false;
	if(MouseLeftAction->GetClass() == add)return true;
	
	if(MouseRightAction == nullptr)return false;
	return MouseRightAction->GetClass() == add;
}

bool AMy_Pawn::CanHoverGround()
{
	UClass* add = AAction_AddTile::StaticClass();
	if(MouseLeftAction == nullptr)return false;
	if(MouseLeftAction->GetClass() == add)return true;
	
	if(MouseRightAction == nullptr)return false;
	return MouseRightAction->GetClass() == add;
}

void AMy_Pawn::UpdateTileTypeUnderCursor(FIntPoint index)
{
	// FIntPoint index = MyGrid->GetTileIndexUnderCursor(GetWorld()->GetFirstPlayerController(),true,CanHoverEmptySpace());
	MyGrid->SetTileTypeByIndex(index,CurSetTileType);
}

TObjectPtr<AMyUnit> AMy_Pawn::GetUnitUnderCursor()
{
	FHitResult HitResult;
	auto playerIndex = GetWorld()->GetFirstPlayerController();
	bool result = playerIndex->GetHitResultUnderCursorByChannel(TraceTypeQuery3,false,HitResult);
	if(result)
	{
		return Cast<AMyUnit>(HitResult.GetActor());
	}
	
	FIntPoint index = MyGrid->GetTileIndexUnderCursor(playerIndex,true,CanHoverEmptySpace());
	const FTileData* pData = MyGrid->GetTileDataByIndex(index);
	return pData ? pData->UnitOnTile : nullptr;
}

void AMy_Pawn::LookAtGrid(const FIntPoint& index)
{
	auto pData = MyGrid->GetTileDataByIndex(index);
	if(pData == nullptr)return;
	m_locationDesired = pData->Transform.GetLocation();
	// m_springArm->SetWorldLocation(pData->Transform.GetLocation());
}

void AMy_Pawn::LookAtUnit(TObjectPtr<AMyUnit> Unit)
{
	LookAtGrid(Unit->GetGridIndex());
}

void AMy_Pawn::StartGame()
{
	IsStartGame = true;
	
	MyGrid->RemoveStateFromTile(HoveredTile,ETileState::Hovered);

	SwitchProcess(BeforeTurnProcess);
}

void AMy_Pawn::SwitchToCmdInput()
{
	SwitchProcess(CmdProcess);
}

void AMy_Pawn::SwitchToNormal()
{
	SwitchProcess(NormalProcess);

}

void AMy_Pawn::SwitchToIdle()
{
	SwitchProcess(IdleProcess);
}

void AMy_Pawn::SwitchToChooseTarget()
{
	SwitchProcess(ChooseTargetProcess);
}

void AMy_Pawn::SwitchToBeforeTurn()
{
	SwitchProcess(BeforeTurnProcess);
}

void AMy_Pawn::SwitchToMove()
{
	SwitchProcess(MoveProcess);
}

void AMy_Pawn::SwitchToCalcAnim()
{
	SwitchProcess(CalcAnimProcess);
}

