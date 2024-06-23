// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "TopDownProjectileAbility.generated.h"

class ATopDownProjectile;
/**
 * 
 */
UCLASS()
class RPG_TOPDOWN_API UTopDownProjectileAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category="Projectile")
	virtual void SpawnProjectile();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile")
	TSubclassOf<ATopDownProjectile> ProjectileClass;
};
