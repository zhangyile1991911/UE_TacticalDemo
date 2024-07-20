// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitPortrait.h"

#include "MyGameInstance.h"
#include "My_Utilities.h"
#include "MyUnit.h"
#include "Components/Image.h"

void UUnitPortrait::RefreshUnitIcon(TObjectPtr<AMyUnit> Unit)
{
	if(GameInstance == nullptr)
	{
		GameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());	
	}	
	auto pData = GameInstance->GetUnitData(Unit->GetUnitType());
	if(pData == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("%d data is null"),Unit->GetUnitType());
		return;
	}
	if(pData->Assets.Icon.IsValid() == false)
	{
		pData->Assets.Icon.LoadSynchronous();
	}
	Portrait->SetBrushFromTexture(pData->Assets.Icon.Get(),false);

	for(auto one : APs)
	{
		one->SetBrushTintColor(FLinearColor::Gray);
	}

	FSlateColor bright(FLinearColor(0.904661f,0.806952f,0.078187f,1.0f));
	for(int i = 0;i < Unit->GetRuntimeProperty().ActionPoint;i++)
	{
		APs[i]->SetBrushTintColor(bright);
	}
	UniqueId = Unit->GetUniqueID();
}

void UUnitPortrait::NativeConstruct()
{
	Super::NativeConstruct();

	APs.Add(AP1);
	APs.Add(AP2);
	APs.Add(AP3);
}

void UUnitPortrait::NativeDestruct()
{
	APs.Empty();
	Super::NativeDestruct();
}

void UUnitPortrait::OnFocus(uint32 UID)
{
	if(UniqueId == UID && !bIsFocus)
	{
		PlayJumpAnim();
		bIsFocus = true;
	}
	else if(bIsFocus)
	{
		PlayFallAnim();
		bIsFocus = false;
	}
}

void UUnitPortrait::PlayJumpAnim()
{
	PlayAnimation(Jump);
}

void UUnitPortrait::PlayFallAnim()
{
	PlayAnimation(Restore);
}
