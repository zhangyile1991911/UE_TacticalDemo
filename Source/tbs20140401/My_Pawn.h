// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridShapeData.h"
#include "InputActionValue.h"
#include "MyButtonList_Units.h"
#include "GameFramework/Pawn.h"
#include "Math/Vector2D.h"

#include "My_Pawn.generated.h"

class UPawnProcess_ChooseTarget;
class UPawnProcess_Idle;
class UPawnProcess_BeforeTurn;
class AMyHUD;
class UPawnProcess_CMD;
class UUPawnProcess_Normal;
class UPawnProcess;
class AMyGridPathfinding;
class AMyUnit;
class AMyCombatSystem;
enum class ETileType;
class AMyAction;
enum class ETileState;
class AGrid;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class UInputAction;

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTileTypeChanged,ETileType,newTipe);

UCLASS()
class TBS20140401_API AMy_Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMy_Pawn();

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> m_springArm;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UCameraComponent> m_camera;
	
	UPROPERTY()
	TObjectPtr<UInputAction> MouseZoomAction;
	UPROPERTY()
	TObjectPtr<UInputAction> CamRotAction;
	UPROPERTY()
	TObjectPtr<UInputAction> CamMoveAction;

	UPROPERTY()
	TObjectPtr<UInputAction> MouseLeftClickAction;
	UPROPERTY()
	TObjectPtr<UInputAction> MouseRightClickAction;
	UPROPERTY()
	TObjectPtr<UInputAction> UnitMoveAction;
	UPROPERTY()
	TObjectPtr<UInputAction> ConfirmAction;
	UPROPERTY()
	TObjectPtr<UInputAction> CancelAction;

	float m_curArmLength;
	FVector m_locationDesired;
	FRotator m_rotationDesired;

	UPROPERTY()
	TObjectPtr<AGrid> MyGrid;

	UPROPERTY()
	TObjectPtr<AMyCombatSystem> MyCombatSystem;

	UPROPERTY()
	TObjectPtr<AMyGridPathfinding> MyGridPathfinding;

	UPROPERTY()
	TObjectPtr<AMyHUD> MyHUDInstance;
	
	FIntPoint HoveredTile = FIntPoint(-1,-1);
	FIntPoint SelectedTile = FIntPoint(-1,-1);

	ETileType CurSetTileType = ETileType::Normal;

	TObjectPtr<AMyUnit> HoveredUnit;

	TObjectPtr<AMyUnit> SelectedUnit;

	UPROPERTY()
	TObjectPtr<UPawnProcess_BeforeTurn> BeforeTurnProcess;
	UPROPERTY()
	TObjectPtr<UPawnProcess_CMD> CmdProcess;
	UPROPERTY()
	TObjectPtr<UUPawnProcess_Normal> NormalProcess;
	UPROPERTY()
	TObjectPtr<UPawnProcess_Idle> IdleProcess;
	UPROPERTY()
	TObjectPtr<UPawnProcess_ChooseTarget> ChooseTargetProcess;
	UPROPERTY()
	TObjectPtr<UPawnProcess> CurrentProcess;

	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Location_Speed = 20.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Location_Interp = 2.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Rotation_Speed = 45.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Rotation_Interp = 2.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Zoom_Speed = 40.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Zoom_Interp = 2.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector2f MaxMin_ArmLength = FVector2f(500.0f,5000.0f);

	UPROPERTY()
	AMyAction* LeftAction = nullptr;

	UPROPERTY()
	AMyAction* RightAction = nullptr;

	UFUNCTION()
	void SetCurrentTileType(ETileType ttype){CurSetTileType = ttype;}

	EUnitType CurrentSelectedUnitType;

	bool IsStartGame = false;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MouseZooming(const FInputActionValue& Value);
	void CamClockWise(const FInputActionValue& Value);
	void CamMove(const FInputActionValue& value);
	void MouseLeftClick(const FInputActionValue& value);
	void MouseRightClick(const FInputActionValue& value);
	void UnitMove(const FInputActionValue& value);
	void ConfirmClick(const FInputActionValue& value);
	void CancelClick(const FInputActionValue& value);

	void SwitchProcess(TObjectPtr<UPawnProcess> NextProcess);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	void UpdateTileUnderCursor();
	void UpdateTileStatusByIndex(const FIntPoint& index,ETileState state);
	void RemoveTileStateByIndex(const FIntPoint& index,ETileState state);
	const FIntPoint& GetHoveredTile() const {return HoveredTile;}
	const FIntPoint& GetSelectedTile() const {return SelectedTile;}
	// void SetSelectedTile(const FIntPoint& point){SelectedTile = point;}
	void SetSelectedActions(UClass* left,UClass* right);
	void AddNewTileUnderCursor();
	// void RemoveTileUnderCursor();
	void RemoveTileUnderCursor(const FIntPoint& index);
	bool CanHoverEmptySpace();
	bool CanHoverGround();
	void UpdateTileTypeUnderCursor(FIntPoint index);

	void SetCurrentSelectedUnitType(EUnitType UnitType){CurrentSelectedUnitType = UnitType;}
	EUnitType GetCurrentSelectedUnitType()const{return CurrentSelectedUnitType;}
	
	TObjectPtr<AGrid> GetMyGrid()const{return MyGrid;}
	TObjectPtr<AMyCombatSystem> GetMyCombatSystem()const{return MyCombatSystem;}
	TObjectPtr<AMyGridPathfinding> GetMyGridPathFinding()const{return MyGridPathfinding;}
	TObjectPtr<AMyHUD> GetMyHUD()const{return MyHUDInstance;}
	TObjectPtr<AMyUnit> GetUnitUnderCursor();

	TObjectPtr<AMyUnit> GetSelectedUnit()const{return SelectedUnit;}

	void LookAtGrid(const FIntPoint&);
	void LookAtUnit(TObjectPtr<AMyUnit>);
	void StartGame();
	void SwitchToCmdInput();
	void SwitchToNormal();
	void SwitchToIdle();
	void SwitchToChooseTarget();
	
	// FTileTypeChanged OnTileTYpeChanged;
};
