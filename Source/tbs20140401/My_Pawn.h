// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "Math/Vector2D.h"

#include "My_Pawn.generated.h"

class AGrid;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class UInputAction;

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

	float m_curArmLength;
	FVector m_locationDesired;
	FRotator m_rotationDesired;

	UPROPERTY()
	AGrid* MyGrid;
	UPROPERTY()
	TObjectPtr<UInputAction> MouseLeftClick;
	UPROPERTY()
	TObjectPtr<UInputAction> MouseRightClick;
	FIntPoint HoveredTile = FIntPoint(-1,-1);
	FIntPoint SelectedTile = FIntPoint(-1,-1);
	
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
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MouseZooming(const FInputActionValue& Value);
	void CamClockWise(const FInputActionValue& Value);
	void CamMove(const FInputActionValue& value);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	void UpdateTileUnderCursor();
	void UpdateTIleByIndex(const FIntPoint& index,ETileState state);
	void RemoveTileStateByIndex(const FIntPoint& index,ETileState state);
	const FIntPoint& GetHoveredTile(){return HoveredTile;}
	const FIntPoint& GetSelectedTile(){return SelectedTile;}
	void SetSelectedActions(UClass* left,UClass* right);
};
