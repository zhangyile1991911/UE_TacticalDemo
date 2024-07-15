// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"

void UGameOverWidget::ShowGameOver()
{
	this->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(Blink,0,0);
}
