// Fill out your copyright notice in the Description page of Project Settings.


#include "HitInfoFlow.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"

void UHitInfoFlow::StartHitNum(FVector2D Start,FVector2D Finish,float Scale,int Num,bool bIsHit,bool bIsCritical,bool bIsBackAtk)
{
	SetVisibility(ESlateVisibility::Visible);
	
	StartPos = Start;
	FinishPos = Finish;
	if(MySlotPtr == nullptr)
	{
		MySlotPtr = Cast<UCanvasPanelSlot>(this->Slot);
		// MySlotPtr = Cast<UCanvasPanelSlot>(this);
	}
	// auto DesiredSize = TextBlockBox->GetDesiredSize();
	// UE_LOG(LogTemp,Log,TEXT("UHitInfoFlow::StartHitNum %f %f"),DesiredSize.X,DesiredSize.Y);
	
	// auto TextSize = MySlotPtr->GetSize();
	// DesiredSize /= 2;
	// DesiredSize /= Scale;
	// StartPos.X -= DesiredSize.X;
	// StartPos.Y -= DesiredSize.Y;
	// FinishPos.X -= DesiredSize.X;
	// FinishPos.Y -= DesiredSize.Y;
	MySlotPtr->SetPosition(StartPos);
	if(bIsHit)
	{
		HitNumTextBlock->SetText(FText::Format(NSLOCTEXT("","","-{0}"),Num));	
	}
	else
	{
		HitNumTextBlock->SetText(FText(NSLOCTEXT("","","MISSING")));
	}
	if(bIsCritical)
		IsCriticalTextBlock->SetVisibility(ESlateVisibility::Visible);
	else
		IsCriticalTextBlock->SetVisibility(ESlateVisibility::Collapsed);

	if(bIsBackAtk)
		IsBackAtkTextBlock->SetVisibility(ESlateVisibility::Visible);
	else
		IsBackAtkTextBlock->SetVisibility(ESlateVisibility::Collapsed);
	
	
	
}

void UHitInfoFlow::UpdateHitNumFlowAnim(float Value)
{
	FVector2D Cur = FMath::Lerp(StartPos,FinishPos,Value);
	MySlotPtr->SetPosition(Cur);
}

void UHitInfoFlow::FinishHitNumFlowAnim()
{
	SetVisibility(ESlateVisibility::Hidden);
}
