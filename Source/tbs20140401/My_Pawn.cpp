// Fill out your copyright notice in the Description page of Project Settings.


#include "My_Pawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"



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
	
}

// Called when the game starts or when spawned
void AMy_Pawn::BeginPlay()
{
	Super::BeginPlay();
	// UE_LOG(LogTemp,Log,TEXT("AMy_Pawn::BeginPlay()"))

	m_curArmLength = 1000.0f;
	m_springArm->TargetArmLength = m_curArmLength;
	m_springArm->SetWorldRotation(FRotator(-60.0f,0.0,0));

	m_locationDesired = GetActorLocation();
	m_rotationDesired = GetActorRotation();
}


// Called every frame
void AMy_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_springArm->TargetArmLength = FMath::FInterpTo(m_springArm->TargetArmLength,m_curArmLength,DeltaTime,Zoom_Interp);
	SetActorLocation(FMath::VInterpTo(GetActorLocation(),m_locationDesired,DeltaTime,Location_Interp));
	SetActorRotation(FMath::RInterpTo(GetActorRotation(),m_rotationDesired,DeltaTime,Rotation_Interp));
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
		
	}
}

void AMy_Pawn::MouseZooming(const FInputActionValue& val)
{
	float tmp = val.Get<FInputActionValue::Axis1D>();
	// UE_LOG(LogTemp,Log,TEXT("AMy_Pawn::MouseZooming %f"),tmp)
	tmp *= Zoom_Speed;
	tmp += m_curArmLength;
	m_curArmLength = FMathf::Clamp(tmp,MaxMin_ArmLength.X,MaxMin_ArmLength.Y);
	
}

void AMy_Pawn::CamClockWise(const FInputActionValue& val)
{
	// UE_LOG(LogTemp,Log,TEXT("AMy_Pawn::CamClockWise"))
	float b1 = val.Get<FInputActionValue::Axis1D>();
	// UE_LOG(LogTemp,Log,TEXT("AMy_Pawn::CamClockWise %f"),b1)
	if(b1 > 0.0f)
	{
		m_rotationDesired.Add(0.0f,Rotation_Speed,0.0f);
	}
	else
	{
		m_rotationDesired.Add(0.0f,-Rotation_Speed,0.0f);
	}
		
}


void AMy_Pawn::CamMove(const FInputActionValue& val)
{
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

