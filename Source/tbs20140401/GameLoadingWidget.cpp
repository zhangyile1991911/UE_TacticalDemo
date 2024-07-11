// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLoadingWidget.h"

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

void UGameLoadingWidget::PlayBlink()
{
	PlayAnimation(Blink,0,0);
}
