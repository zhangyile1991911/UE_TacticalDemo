﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystemPanel.h"

#include "GameLoadingWidget.h"

void UGameSystemPanel::ShowLoading()
{
	this->SetVisibility(ESlateVisibility::Visible);
	GameLoading->SetVisibility(ESlateVisibility::Visible);
	GameLoading->PlayLoading();
}

void UGameSystemPanel::HideLoading()
{
	GameLoading->SetVisibility(ESlateVisibility::Hidden);
	GameLoading->StopBlink();
	GameLoading->StopLoading();
	// GameLoading->StopAllAnimations();
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UGameSystemPanel::WaitingEnter()
{
	this->SetVisibility(ESlateVisibility::Visible);
	GameLoading->SetVisibility(ESlateVisibility::Visible);
	GameLoading->StopLoading();
	GameLoading->PlayBlink();
}