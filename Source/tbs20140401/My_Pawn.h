// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridShapeData.h"
#include "InputActionValue.h"
#include "MyButtonList_Units.h"
#include "GameFramework/Pawn.h"
#include "Math/Vector2D.h"

#include "My_Pawn.generated.h"

class UPawnProcess_FinishTurn;
class ADemoControl;
class UPawnProcess_LoadFailed;
class UPawnProcess_LoadStage;
class UPawnProcess_Focus;
class UEventCenter;
class UPawnProcess_CalcAnim;
class UPawnProcess_Move;
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
class APathPointInst;
class AStoryTeller;
class UPawnProcess_Story;
class AMyLevelLoading;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCameraEvent);

UENUM(BlueprintType)
enum class ECameraDirectType:uint8
{
	RIGHT UMETA(DisplayName="Right"),
	LEFT UMETA(DisplayName="Left"),
	FORWARD UMETA(DisplayName="Forward"),
	BACKWARD UMETA(DisplayName="Backward")
};
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTileTypeChanged,ETileType,newTipe);

UCLASS(Blueprintable)
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
	TObjectPtr<UEventCenter> EventCenter;
	
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
	UPROPERTY()
	TObjectPtr<UInputAction> SpaceAction;
	UPROPERTY()
	TObjectPtr<UInputAction> TabAction;
	UPROPERTY()
	TObjectPtr<UInputAction> KeyBoardLeftAction;
	UPROPERTY()
	TObjectPtr<UInputAction> KeyBoardRightAction;

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
	TObjectPtr<APathPointInst> MyPathPointInst;

	UPROPERTY()
	TObjectPtr<AStoryTeller> MyStoryTeller;

	UPROPERTY()
	TObjectPtr<AMyLevelLoading> MyLevelLoad;
	
	UPROPERTY()
	TObjectPtr<AMyHUD> MyHUDInstance;
	
	FIntPoint HoveredTile = FIntPoint(-1,-1);
	FIntPoint SelectedTile = FIntPoint(-1,-1);

	ETileType CurSetTileType = ETileType::Normal;

	UPROPERTY()
	TObjectPtr<AMyUnit> HoveredUnit;

	UPROPERTY()
	TObjectPtr<AMyUnit> SelectedUnit;

	UPROPERTY()
	TObjectPtr<UPawnProcess_Move> MoveProcess;
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
	TObjectPtr<UPawnProcess_CalcAnim> CalcAnimProcess;
	UPROPERTY()
	TObjectPtr<UPawnProcess_Story> StoryProcess;
	UPROPERTY()
	TObjectPtr<UPawnProcess_LoadStage> LoadStageProcess;
	UPROPERTY()
	TObjectPtr<UPawnProcess_LoadFailed> LoadFailedProcess;
	UPROPERTY()
	TObjectPtr<UPawnProcess_FinishTurn> FinishTurnProcess;
	UPROPERTY()
	TObjectPtr<UPawnProcess> CurrentProcess;

	ECameraDirectType CameraDirect = ECameraDirectType::FORWARD;
	
	bool bHasReadStory = false;

	int StageLevelNum = 1;

	FTimerHandle TimerStartGameHandle;
	
	void DelayStartGame();
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
	FVector2f MaxMin_ArmLength = FVector2f(1000.0f,3000.0f);

	UPROPERTY()
	AMyAction* MouseLeftAction = nullptr;

	UPROPERTY()
	AMyAction* MouseRightAction = nullptr;

	ECameraDirectType GetCurrentCameraDirect()const{return CameraDirect;}	

	UFUNCTION()
	void SetCurrentTileType(ETileType ttype){CurSetTileType = ttype;}

	UFUNCTION(BlueprintCallable)
	void Init();

	ETBSUnitType CurrentSelectedUnitType;

	bool IsStartGame = false;
	int GetStageLevelNum()const{return StageLevelNum;}
	void NextStageLevel(){StageLevelNum++;}
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
	void SpaceClick(const FInputActionValue& value);
	void TabClick(const FInputActionValue& value);
	void LeftClick(const FInputActionValue& value);
	void RightClick(const FInputActionValue& value);

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

	void SetCurrentSelectedUnitType(ETBSUnitType UnitType){CurrentSelectedUnitType = UnitType;}
	ETBSUnitType GetCurrentSelectedUnitType()const{return CurrentSelectedUnitType;}

	// UFUNCTION(BlueprintCallable)
	// AGrid* GetMyGridForBP()const{return MyGrid;}
	TObjectPtr<AStoryTeller> GetMyStoryTeller();
	TObjectPtr<AGrid> GetMyGrid();
	TObjectPtr<AMyCombatSystem> GetMyCombatSystem();
	TObjectPtr<AMyGridPathfinding> GetMyGridPathFinding();
	TObjectPtr<AMyLevelLoading> GetMyLevelLoading();
	
	TObjectPtr<AMyHUD> GetMyHUD()const{return MyHUDInstance;}
	UFUNCTION(BlueprintCallable)
	AMyHUD* GetMyHUDForBP()const{return MyHUDInstance;}
	
	TObjectPtr<AMyUnit> GetUnitUnderCursor();

	TObjectPtr<AMyUnit> GetSelectedUnit()const{return SelectedUnit;}
	TObjectPtr<APathPointInst> GetMyPathPointInst();

	TObjectPtr<UEventCenter> GetEventCenter()const{return EventCenter;}

	void CameraControlLeft()
	{
		m_rotationDesired.Add(0.0f,Rotation_Speed,0.0f);
	}
	void CameraControlRight()
	{
		m_rotationDesired.Add(0.0f,-Rotation_Speed,0.0f);
	}
	void CameraControlZooming(float val)
	{
		val *= Zoom_Speed;
		val += m_curArmLength;
		m_curArmLength = FMathf::Clamp(val,MaxMin_ArmLength.X,MaxMin_ArmLength.Y);
	}

	void ClearCurTurnData();
	
	bool ReadStory()const{return bHasReadStory;}
	void SetReadStory(){bHasReadStory = true;}
	
	void LookAtGrid(const FIntPoint&);
	void LookAtUnit(TObjectPtr<AMyUnit>);
	void StartGame(bool bIsDemo);
	void SwitchToCmdInput();
	void SwitchToNormal();
	void SwitchToIdle();
	void SwitchToChooseTarget();
	void SwitchToBeforeTurn();
	void SwitchToMove();
	void SwitchToCalcAnim();
	void SwitchToTellStory();
	void SwitchToLoadStage();
	void SwitchToLoadFailed();
	void SwitchToFinishTurn();
	
	FCameraEvent OnCameraActing;
};
