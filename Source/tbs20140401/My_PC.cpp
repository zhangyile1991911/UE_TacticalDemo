// Fill out your copyright notice in the Description page of Project Settings.


#include "My_PC.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
AMy_PC::AMy_PC()
{
	UE_LOG(LogTemp, Log, TEXT("AMy_PC::AMy_PC()"));
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingAsset(TEXT("InputMappingContext'/Game/Input/NewInputMappingContext'"));
	if (InputMappingAsset.Succeeded())
	{
		DefaultMappingContext = InputMappingAsset.Object;
	}
}

void AMy_PC::BeginPlay()
{
	Super::BeginPlay();
	SetShowMouseCursor(true);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		UE_LOG(LogTemp, Log, TEXT("AMy_PC::BeginPlay()"));
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}
