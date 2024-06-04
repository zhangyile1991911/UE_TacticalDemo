// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUI_BattleInfo.h"

#include "MyUnit.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"


void UGameUI_BattleInfo::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UGameUI_BattleInfo::NativeDestruct()
{
	Super::NativeDestruct();
	
}

void UGameUI_BattleInfo::StartHitNumFlowAnim(AMyUnit* Unit,int Num)
{
	if(Unit == nullptr)return;
	
	FVector2D ScreenLocation;
	const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	const FVector WorldPosition =Unit->GetActorLocation();
	
	const bool Result = PlayerController->ProjectWorldLocationToScreen(WorldPosition,ScreenLocation,true);
	if(Result)
	{
		const float Scale = UWidgetLayoutLibrary::GetViewportScale(PlayerController);
		ScreenLocation /= Scale;
		HitNum->SetText(FText::Format(NSLOCTEXT("","","-{0}"),Num));
		HitNumSlot = Cast<UCanvasPanelSlot>(HitNum->Slot);
		auto TextSize = HitNumSlot->GetSize();
		TextSize /= 2;
		TextSize /= Scale;
		ScreenLocation.X -= TextSize.X;
		ScreenLocation.Y -= TextSize.Y;
		HitNumSlot->SetPosition(ScreenLocation);
		StartFlowPosition = ScreenLocation;
		ScreenLocation.Y -= 100;
		FinishFlowPosition = ScreenLocation;
	}
	// UE_LOG(LogTemp,Log,TEXT("Target Location = %s Result = %hhd ScreenLocation = %s HitNum = %d"),*WorldPosition.ToString(),Result,*ScreenLocation.ToString(),Num)
	HitNum->SetVisibility(ESlateVisibility::Visible);	
}

void UGameUI_BattleInfo::UpdateHitNumFlowAnim(float Value)
{
	if(HitNumSlot)
	{
		FVector2D Cur = FMath::Lerp(StartFlowPosition,FinishFlowPosition,Value);
		HitNumSlot->SetPosition(Cur);	
	}
}

void UGameUI_BattleInfo::FinishHitNumFlowAnim()
{
	HitNum->SetVisibility(ESlateVisibility::Hidden);	
}

