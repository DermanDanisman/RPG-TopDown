// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "TopDownAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class RPG_TOPDOWN_API UTopDownAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	static UTopDownAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
};
