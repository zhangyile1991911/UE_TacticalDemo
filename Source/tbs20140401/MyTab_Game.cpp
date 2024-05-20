// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTab_Game.h"

#include "Components/Button.h"
#include "GameFramework/HUD.h"
#include "MyHUD.h"
#include "My_Pawn.h"


void UMyTab_Game::NativeConstruct()
{
	Super::NativeConstruct();

	StartGameBtn->OnClicked.AddDynamic(this,&UMyTab_Game::OnStartGameClicked);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		// 获取当前的 HUD 实例
		AHUD* HUD = PlayerController->GetHUD();
		if (HUD)
		{
			// 尝试将 HUD 转换为你的自定义 HUD 类
			MyHUDInstance = Cast<AMyHUD>(HUD);
		}

		APawn* PAWN = PlayerController->GetPawn();
		if(PAWN)
		{
			MyPawnInstance = Cast<AMy_Pawn>(PAWN);
		}
	}
	
}

void UMyTab_Game::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMyTab_Game::OnStartGameClicked()
{
	MyHUDInstance->ShowGameUI(true);
	MyPawnInstance->SetSelectedActions(nullptr,nullptr);
	
}
