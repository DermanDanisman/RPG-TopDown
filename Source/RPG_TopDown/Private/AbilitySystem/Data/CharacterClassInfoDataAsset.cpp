// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/CharacterClassInfoDataAsset.h"

FCharacterClassDefaultInfo UCharacterClassInfoDataAsset::GetCharacterClassDefaultInfo(ECharacterClass CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}
