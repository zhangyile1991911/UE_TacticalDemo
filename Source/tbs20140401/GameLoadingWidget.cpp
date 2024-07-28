// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLoadingWidget.h"

#include "Components/Image.h"

void UGameLoadingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Tutorial->SetVisibility(ESlateVisibility::Collapsed);
}

void UGameLoadingWidget::PlayLoading()
{
	PlayAnimation(Loading,0,0);
}

void UGameLoadingWidget::StopLoading()
{
	StopAnimation(Loading);
}

void UGameLoadingWidget::StopBlink()
{
	StopAnimation(Blink);
}

void UGameLoadingWidget::ShowTutorial()
{
	Tutorial->SetVisibility(ESlateVisibility::Visible);
}

void UGameLoadingWidget::HideTutorial()
{
	Tutorial->SetVisibility(ESlateVisibility::Collapsed);
}

void UGameLoadingWidget::PlayBlink()
{
	PlayAnimation(Blink,0,0);
}
