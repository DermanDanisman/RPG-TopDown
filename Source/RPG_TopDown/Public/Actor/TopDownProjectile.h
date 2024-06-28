// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "TopDownProjectile.generated.h"



/* Forward Declaration */
class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraSystem;
struct FGameplayEffectSpecHandle;

UCLASS()
class RPG_TOPDOWN_API ATopDownProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	ATopDownProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

protected:
	
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere, Category="Effects")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere, Category="Effects")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere, Category="Effects")
	TObjectPtr<USoundBase> LoopingEffectSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingEffectAudioComponent;

	bool bCollisionHit = false;

	UPROPERTY(EditAnywhere, Category="Effects")
	float LifeSpan = 2.5f;

};
