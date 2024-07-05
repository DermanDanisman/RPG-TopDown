// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfoDataAsset.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass : uint8 
{
	Warrior,
	Ranger,
	Sorcerer
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	// Now in this struct we're obviously going to need a gameplay effect to apply our primary attributes.
	UPROPERTY(EditDefaultsOnly, Category="Class Defaults|Attributes")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
};


/**
 * 
 */
UCLASS()
class RPG_TOPDOWN_API UCharacterClassInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	/*
	 * Now our data asset needs a way to store these structs one for each one of our classes, so we can use a TArray.
	 * Or we could use a TMap. A TMap is nice because we could map the enum to the struct.
	 */
	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;
	FCharacterClassDefaultInfo GetCharacterClassDefaultInfo(ECharacterClass CharacterClass);

	/* Shared Attributes */
	/*
	 * Now we could have secondary and vital attribute gameplay effects per class, or
	 * we can share the same secondary and primary attributes gameplay effect among all classes.
	 * That's really up to us. I'd like to share those
	 */
	UPROPERTY(EditDefaultsOnly, Category="Common Class Defaults|Attributes")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;
	
	UPROPERTY(EditDefaultsOnly, Category="Common Class Defaults|Attributes")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Common Class Defaults|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> CommonGameplayAbilities;
};
