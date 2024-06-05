// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUI_BattleInfo.h"

#include "MyUnit.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"


void UGameUI_BattleInfo::NativeConstruct()
{
	Super::NativeConstruct();
	HitNum->SetVisibility(ESlateVisibility::Hidden);
	BackAtkTips->SetVisibility(ESlateVisibility::Hidden);
	CooperationTips->SetVisibility(ESlateVisibility::Hidden);

	TipsGroupSlot = Cast<UCanvasPanelSlot>(TipsGroup->Slot);
}

void UGameUI_BattleInfo::NativeDestruct()
{
	Super::NativeDestruct();
	
}

bool UGameUI_BattleInfo::LocationToScreenPosition(FVector UnitLocation,FVector2D& ScreenLocation,float& Scale)
{
	const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	const bool Result = PlayerController->ProjectWorldLocationToScreen(UnitLocation,ScreenLocation,true);
	if(Result)
	{
		Scale = UWidgetLayoutLibrary::GetViewportScale(PlayerController);
		ScreenLocation /= Scale;
	}
	return Result;
}

void UGameUI_BattleInfo::StartHitNumFlowAnim(AMyUnit* Unit,int Num,bool IsHit)
{
	if(Unit == nullptr)return;
	
	FVector2D ScreenLocation;
	float Scale;
	const bool Result = LocationToScreenPosition(Unit->GetActorLocation(),ScreenLocation,Scale);
	if(Result)
	{
		if(IsHit)
		{
			HitNum->SetText(FText::Format(NSLOCTEXT("","","-{0}"),Num));	
		}
		else
		{
			HitNum->SetText(FText(NSLOCTEXT("","","MISSING")));
		}
		
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
		HitNum->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		HitNum->SetVisibility(ESlateVisibility::Hidden);
	}
	// const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	//
	// const FVector WorldPosition =Unit->GetActorLocation();
	
	// const bool Result = PlayerController->ProjectWorldLocationToScreen(WorldPosition,ScreenLocation,true);
	// if(Result)
	// {
	// 	const float Scale = UWidgetLayoutLibrary::GetViewportScale(PlayerController);
	// 	ScreenLocation /= Scale;
	// 	HitNum->SetText(FText::Format(NSLOCTEXT("","","-{0}"),Num));
	// 	HitNumSlot = Cast<UCanvasPanelSlot>(HitNum->Slot);
	// 	auto TextSize = HitNumSlot->GetSize();
	// 	TextSize /= 2;
	// 	TextSize /= Scale;
	// 	ScreenLocation.X -= TextSize.X;
	// 	ScreenLocation.Y -= TextSize.Y;
	// 	HitNumSlot->SetPosition(ScreenLocation);
	// 	StartFlowPosition = ScreenLocation;
	// 	ScreenLocation.Y -= 100;
	// 	FinishFlowPosition = ScreenLocation;
	// }
	// UE_LOG(LogTemp,Log,TEXT("Target Location = %s Result = %hhd ScreenLocation = %s HitNum = %d"),*WorldPosition.ToString(),Result,*ScreenLocation.ToString(),Num)
	// HitNum->SetVisibility(ESlateVisibility::Visible);	
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

void UGameUI_BattleInfo::ShowBackAtkTips(AMyUnit* Unit)
{
	FVector2D ScreenPosition;
	float Scale;
	const bool bShow = LocationToScreenPosition(Unit->GetActorLocation(),ScreenPosition,Scale);
	if(bShow)
	{
		auto TextSize = TipsGroupSlot->GetSize();
		TextSize /= 2;
		TextSize /= Scale;
		ScreenPosition.X -= TextSize.X;
		ScreenPosition.Y -= TextSize.Y;
		TipsGroupSlot->SetPosition(ScreenPosition);
		BackAtkTips->SetVisibility(ESlateVisibility::Visible);	
	}
}

void UGameUI_BattleInfo::HideBackAtkTips()
{
	BackAtkTips->SetVisibility(ESlateVisibility::Hidden);
}

void UGameUI_BattleInfo::ShowCooperatorTips(AMyUnit* Unit)
{
	FVector2D ScreenPosition;
	float Scale;
	const bool bShow = LocationToScreenPosition(Unit->GetActorLocation(),ScreenPosition,Scale);
	if(bShow)
	{
		auto TextSize = TipsGroupSlot->GetSize();
		TextSize /= 2;
		TextSize /= Scale;
		ScreenPosition.X -= TextSize.X;
		ScreenPosition.Y -= TextSize.Y;
		TipsGroupSlot->SetPosition(ScreenPosition);
		CooperationTips->SetVisibility(ESlateVisibility::Visible);	
	}
}

void UGameUI_BattleInfo::HideCooperatorTips()
{
	CooperationTips->SetVisibility(ESlateVisibility::Hidden);
}


