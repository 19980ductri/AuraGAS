// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParam)
{
	PlayerController = WCParam.PlayerController;
	PlayerState = WCParam.PlayerState;
	AbilitySystemComponent = WCParam.AbilitySystemComponent;
	AttributeSet = WCParam.Attributes;
}

void UAuraWidgetController::BroadCastInitialValue()
{
	
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
	
}
