﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitAbility.h"
#include "UnitAbility_NormalAttack.generated.h"

/**
 * 
 */
UCLASS()
class TBS20140401_API UUnitAbility_NormalAttack : public UUnitAbility
{
	GENERATED_BODY()
public:
	virtual bool CanExecute() override;
};
