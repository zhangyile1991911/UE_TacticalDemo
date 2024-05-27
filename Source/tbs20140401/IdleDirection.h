// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IdleDirection.generated.h"

class UArrowComponent;

UCLASS()
class TBS20140401_API AIdleDirection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIdleDirection();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root;
	
	// UPROPERTY(VisibleAnywhere)
	// TObjectPtr<UArrowComponent> Arrow;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> UPArrow;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> LeftArrow;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> DownArrow;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> RightArrow;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bShow;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void DoUpArrowAnimation();

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void DoDownArrowAnimation();

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void DoLeftArrowAnimation();

	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
    void DoRightArrowAnimation();

	void ShowArrow(){bShow = true;}
	void HideArrow(){bShow = false;}
};
