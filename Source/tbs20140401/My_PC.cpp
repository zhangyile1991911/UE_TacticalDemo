// Fill out your copyright notice in the Description page of Project Settings.


#include "My_PC.h"

#include "EnhancedInputSubsystems.h"
#include "Grid.h"
#include "InputMappingContext.h"
#include "My_Pawn.h"
#include "MyCombatSystem.h"
#include "MyGridPathfinding.h"
#include "MyUnit.h"

AMy_PC::AMy_PC()
{
	// UE_LOG(LogTemp, Log, TEXT("AMy_PC::AMy_PC()"));
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

	// APawn* MyPawn = GetPawn();
	// MyPawnInstance = Cast<AMy_Pawn>(MyPawn);
}

void AMy_PC::PushCommand(FCommand cmd)
{
	Commands.Add(cmd);
}

FCommand AMy_PC::PopCommand()
{
	if(Commands.Num() > CommandIndex)
	{
		return Commands[CommandIndex++];	
	}
	FCommand cmd;
	return MoveTemp(cmd);
}

void AMy_PC::ClearCommand()
{
	Commands.Empty();
	CommandIndex = 0;
}

void AMy_PC::StartUnitTurn(TObjectPtr<AMyUnit> Unit)
{
	// CurrentControlUnit = Unit;
	//更新下当前选中格子
	// MyPawnInstance->GetMyGrid()->AddStateToTile(CurrentControlUnit->GetGridIndex(),ETileState::Selected);
	//看向当前行动单位
	// MyPawnInstance->LookAtUnit(Unit);
	//显示当前行动单位可移动范围
	// TArray<FIntPoint> WalkableTiles = MyPawnInstance->GetMyGridPathFinding()->UnitWalkablePath(
	// 	Unit->GetGridIndex(),
	// 	Unit->GetProperty().WalkRange,
	// 	Unit->UnitCanWalkTileType());
	// for(const FIntPoint& one : WalkableTiles)
	// {
	// 	MyPawnInstance->GetMyGrid()->AddStateToTile(one,ETileState::Reachable);	
	// }
	
}

void AMy_PC::FinishUnitTurn()
{
	// MyPawnInstance->GetMyCombatSystem()->NextAllUnit();
	// StartUnitTurn(MyPawnInstance->GetMyCombatSystem()->SortActionPriority());
}

// TObjectPtr<AMyUnit> AMy_PC::GetCurrentControlUnit()const
// {
// 	// return CurrentControlUnit;
// }
