// Fill out your copyright notice in the Description page of Project Settings.


#include "MyButtonList_Units.h"

#include "ButtonUnit.h"


void UMyButtonList_Units::NativePreConstruct()
{
	Super::NativePreConstruct();
	UE_LOG(LogTemp,Log,TEXT("UMyButtonList_Units::NativePreConstruct"))

	// FSoftObjectPath MyAssetPath(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/DebugMenu/Widgets/BP_Button_Unit.BP_Button_Unit'"));
	// FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	// auto rootWidget = Cast<UUserWidget>(Streamable.LoadSynchronous(MyAssetPath));
	//Script/UMGEditor.WidgetBlueprint/Script/UMGEditor.WidgetBlueprint'/Game/DebugMenu/Widgets/BP_Button_Unit.BP_Button_Unit''/Game/DebugMenu/Widgets/BP_Button_Unit.BP_Button_Unit'
	// static ConstructorHelpers::FClassFinder<UUserWidget> ButtonBP(TEXT("/Game/DebugMenu/Widgets/BP_Button_Unit.BP_Button_Unit"));
	// if (!ButtonBP.Succeeded())
	// {
	// 	return;
	// }
	// auto bclass = ButtonBP.Class.Get();
	// if(MyButtonBlueprint == nullptr)return;
	// auto widget = CreateWidget<UUserWidget>(this,MyButtonBlueprint->GetClass(),TEXT("Warrior"));
	// if(widget != nullptr)
	// {
	// 	// widget->SetButtonStyle(EUnitType::Warrior,IsLookingRight);
	// 	// ButtonUnits.Add(TObjectPtr<UButtonUnit>(widget));
	// 	// hBox_Units->AddChildToHorizontalBox(widget);	
	// }
	//
	//
	// widget = CreateWidget<UButtonUnit>(this,MyButtonBlueprint.Get()->GetClass(),TEXT("Ranger"));
	// if(widget != nullptr)
	// {
	// 	// widget->SetButtonStyle(EUnitType::Ranger,IsLookingRight);
	// 	// ButtonUnits.Add(TObjectPtr<UButtonUnit>(widget));
	// 	// hBox_Units->AddChildToHorizontalBox(widget);	
	// }
	//
	//
	// widget = CreateWidget<UButtonUnit>(this,MyButtonBlueprint.Get()->GetClass(),TEXT("Slime"));
	// if(widget != nullptr)
	// {
	// 	// widget->SetButtonStyle(EUnitType::Slime,IsLookingRight);
	// 	// ButtonUnits.Add(TObjectPtr<UButtonUnit>(widget));
	// 	// hBox_Units->AddChildToHorizontalBox(widget);	
	// }
	
}

void UMyButtonList_Units::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp,Log,TEXT("UMyButtonList_Units::NativeConstruct"))

	
	for(auto one : ButtonUnits)
	{
		one->OnClickTopic.BindDynamic(this,&UMyButtonList_Units::OnClickUnit);
	}
	
}

void UMyButtonList_Units::OnClickUnit(UButtonUnit* param)
{
	for(auto one : ButtonUnits)
	{
		if(one == param)
		{
			one->SetIsSelected(true);
			CurrentSelectedButton = param;
			if(OnUnitTypeChanged.IsBound())
				OnUnitTypeChanged.Execute(one->GetUnitType());
		}
		else
		{
			one->SetIsSelected(false);
		}
	}
}
