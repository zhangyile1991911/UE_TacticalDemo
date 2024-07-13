// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUI_BattleInfo.h"

#include "HitInfoFlow.h"
#include "MyUnit.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanel.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "My_Pawn.h"


void UGameUI_BattleInfo::NativeConstruct()
{
	Super::NativeConstruct();
	
	FSoftObjectPath MyAssetPath(TEXT("/Game/DebugMenu/Widgets/ActionBar/BP_HitInfoFlow.BP_HitInfoFlow_C"));
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	ChildWidgetClass = Cast<UClass>(Streamable.LoadSynchronous(MyAssetPath));
	if (ChildWidgetClass != nullptr)
	{
		HitInfoPool.Reserve(5);
		for (int i = 0;i < 5;i++)
		{
			auto UWidget = CreateWidget<UUserWidget>(CanvasPanel,ChildWidgetClass);
			CanvasPanel->AddChildToCanvas(UWidget);
			auto HitInfo = Cast<UHitInfoFlow>(UWidget);
			HitInfo->SetVisibility(ESlateVisibility::Hidden);
			HitInfoPool.Add(HitInfo);
			HitInfo->SetAlignmentInViewport(FVector2D(0.5f,0.5f));
		}
	}
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
	if(PlayerControllerPtr == nullptr)
	{
		PlayerControllerPtr = GetWorld()->GetFirstPlayerController();
	}
	
	const bool Result = PlayerControllerPtr->ProjectWorldLocationToScreen(UnitLocation,ScreenLocation,true);
	if(Result)
	{
		Scale = UWidgetLayoutLibrary::GetViewportScale(PlayerControllerPtr);
		ScreenLocation /= Scale;
	}
	return Result;
}


void UGameUI_BattleInfo::StartHitNumFlowAnim(AMyUnit* Unit,int Num,bool bIsHit,bool bIsCritical,bool bIsBackAtk)
{
	if(Unit == nullptr)return;
	
	UsingHitInfos.Add(HitInfoPool.Last());
	HitInfoPool.RemoveAt(HitInfoPool.Num() - 1);
	
	auto OneHit = UsingHitInfos.Last();
	
	FVector2D ScreenLocation;
	float Scale;
	const bool Result = LocationToScreenPosition(Unit->GetActorLocation(),ScreenLocation,Scale);
	if(Result)
	{
		FVector2D StartFlowPosition;
		FVector2D FinishFlowPosition;
		
		StartFlowPosition = ScreenLocation;
		ScreenLocation.Y -= 100;
		FinishFlowPosition = ScreenLocation;
		
		OneHit->StartHitNum(StartFlowPosition,FinishFlowPosition,Scale,Num,bIsHit,bIsCritical,bIsBackAtk);
		OneHit->SetVisibility(ESlateVisibility::Visible);
	}
}

void UGameUI_BattleInfo::UpdateHitNumFlowAnim(float Value)
{
	for(int i = 0;i < UsingHitInfos.Num();i++)
	{
		UsingHitInfos[i]->UpdateHitNumFlowAnim(Value);
	}
}

void UGameUI_BattleInfo::FinishHitNumFlowAnim()
{
	if(UsingHitInfos.IsEmpty())
	{
		return;
	}
	auto One = UsingHitInfos.Last();
	One->FinishHitNumFlowAnim();
	HitInfoPool.Add(One);
	UsingHitInfos.RemoveAt(UsingHitInfos.Num() - 1);
	One->SetVisibility(ESlateVisibility::Hidden);
}

void UGameUI_BattleInfo::ShowBackAtkTips(AMyUnit* Unit)
{
	const bool bShow = UpdateWidgetPosition(Unit->GetActorLocation());
	if(bShow)
	{
		BackAtkTips->SetVisibility(ESlateVisibility::Visible);
		BackAtkBorder->SetVisibility(ESlateVisibility::Visible);
	}
	// FVector2D ScreenPosition;
	// float Scale;
	// const bool bShow = LocationToScreenPosition(Unit->GetActorLocation(),ScreenPosition,Scale);
	// if(bShow)
	// {
	// 	auto TextSize = TipsGroupSlot->GetSize();
	// 	TextSize /= 2;
	// 	TextSize /= Scale;
	// 	ScreenPosition.X -= TextSize.X;
	// 	ScreenPosition.Y -= TextSize.Y;
	// 	TipsGroupSlot->SetPosition(ScreenPosition);
	// 	BackAtkTips->SetVisibility(ESlateVisibility::Visible);
	// 	BackAtkBorder->SetVisibility(ESlateVisibility::Visible);
	// }
}

void UGameUI_BattleInfo::HideBackAtkTips()
{
	BackAtkTips->SetVisibility(ESlateVisibility::Hidden);
	BackAtkBorder->SetVisibility(ESlateVisibility::Hidden);
}

void UGameUI_BattleInfo::ShowCooperatorTips(AMyUnit* Unit)
{
	const bool bShow = UpdateWidgetPosition(Unit->GetActorLocation());
	if(bShow)
	{
		CooperationTips->SetVisibility(ESlateVisibility::Visible);	
		CooperationBorder->SetVisibility(ESlateVisibility::Visible);
	}
	// FVector2D ScreenPosition;
	// float Scale;
	// const bool bShow = LocationToScreenPosition(Unit->GetActorLocation(),ScreenPosition,Scale);
	// if(bShow)
	// {
	// 	auto TextSize = TipsGroupSlot->GetSize();
	// 	TextSize /= 2;
	// 	TextSize /= Scale;
	// 	ScreenPosition.X -= TextSize.X;
	// 	ScreenPosition.Y -= TextSize.Y;
	// 	TipsGroupSlot->SetPosition(ScreenPosition);
	// 	CooperationTips->SetVisibility(ESlateVisibility::Visible);	
	// 	CooperationBorder->SetVisibility(ESlateVisibility::Visible);
	// }
}

void UGameUI_BattleInfo::HideCooperatorTips()
{
	CooperationTips->SetVisibility(ESlateVisibility::Hidden);
	CooperationBorder->SetVisibility(ESlateVisibility::Hidden);
}

bool UGameUI_BattleInfo::UpdateWidgetPosition(FVector WorldLocation)
{
	FVector2D ScreenPosition;
	float Scale;
	const bool bShow = LocationToScreenPosition(WorldLocation,ScreenPosition,Scale);
	if(bShow)
	{
		auto TextSize = TipsGroupSlot->GetSize();
		TextSize /= 2;
		TextSize /= Scale;
		ScreenPosition.X += TextSize.X;
		ScreenPosition.Y -= TextSize.Y;
		TipsGroupSlot->SetPosition(ScreenPosition);
	}
	return bShow;
}


