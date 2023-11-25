// Fill out your copyright notice in the Description page of Project Settings.


#include "Interact/CombatInterface.h"

// Add default functionality here for any ICombatInterface functions that are not pure virtual.
int32 ICombatInterface::GetPlayerLevel()
{
	return 0;
}

FVector ICombatInterface::GetFacingTarget()
{
	return FVector();
}