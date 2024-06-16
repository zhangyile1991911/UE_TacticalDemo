// Fill out your copyright notice in the Description page of Project Settings.


#include "HitInfoFlow.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"

void UHitInfoFlow::StartHitNum(FVector2D Start,FVector2D Finish,float Scale,int Num,bool IsHit)
{
	SetVisibility(ESlateVisibility::Visible);
	
	StartPos = Start;
	FinishPos = Finish;
	if(MySlotPtr == nullptr)
	{
		MySlotPtr = Cast<UCanvasPanelSlot>(this->Slot);
		// MySlotPtr = Cast<UCanvasPanelSlot>(this);
	}
	auto TextSize = MySlotPtr->GetSize();
	TextSize /= 2;
	TextSize /= Scale;
	StartPos.X -= TextSize.X;
	StartPos.Y -= TextSize.Y;
	MySlotPtr->SetPosition(StartPos);
	if(IsHit)
	{
		HitNum->SetText(FText::Format(NSLOCTEXT("","","-{0}"),Num));	
	}
	else
	{
		HitNum->SetText(FText(NSLOCTEXT("","","MISSING")));
	}
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
