// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/Interaction/CombatInterface.h"

// Add default functionality here for any ICombatInterface functions that are not pure virtual.
int32 ICombatInterface::GetCharacterLevel()
{
	return 0;
}

ECharacterClass ICombatInterface::GetCharacterClass()
{
	return ECharacterClass::Warrior;
}

FVector ICombatInterface::GetWeaponTipSocketLocation()
{
	return FVector();
}
