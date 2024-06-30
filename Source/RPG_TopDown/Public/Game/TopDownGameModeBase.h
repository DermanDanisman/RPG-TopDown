// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TopDownGameModeBase.generated.h"

class UCharacterClassInfoDataAsset;
/**
 * 
 */
UCLASS()
class RPG_TOPDOWN_API ATopDownGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults")
	TObjectPtr<UCharacterClassInfoDataAsset> CharacterClassInfoDataAsset;
};
