// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/TopDownProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ATopDownProjectile::ATopDownProjectile()
{
 	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultSceneRoot);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
	Sphere->SetCollisionProfileName(FName("Projectile"));

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
}

void ATopDownProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Configures the collision sphere to ignore the instigator (the actor that spawned the projectile).
	Sphere->IgnoreActorWhenMoving(GetInstigator(), true);

	SetLifeSpan(LifeSpan);
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ATopDownProjectile::OnSphereOverlap);

	ensure(LoopingEffectSound);
	const EAttachLocation::Type AttachLocationType = EAttachLocation::KeepWorldPosition;
	LoopingEffectAudioComponent = UGameplayStatics::SpawnSoundAttached(LoopingEffectSound, Sphere, FName(), GetActorLocation(), FRotator().ZeroRotator, AttachLocationType);
}

void ATopDownProjectile::Destroyed()
{
	if (!bCollisionHit && !HasAuthority())
	{
		ensure(ImpactSound);
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator().ZeroRotator);
		ensure(ImpactEffect);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		
		if (LoopingEffectAudioComponent)
		{
			// WARNING: nullptr error on dedicated server.
			LoopingEffectAudioComponent->Stop();
		}
	}
	Super::Destroyed();
}

void ATopDownProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ensure(ImpactSound);
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator().ZeroRotator);
	ensure(ImpactEffect);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	
	if (LoopingEffectAudioComponent)
	{
		// WARNING: nullptr error on dedicated server.
		LoopingEffectAudioComponent->Stop();
	}
	
	if (HasAuthority())
	{
		// Now Applying Gameplay Effect is something we should do on the server only as we don't need to do this on clients.
		// The effect is going to change an attribute and the attribute itself is replicated.
		// So the end result, the changing of the attribute that will be replicated.
		if (UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			/*
			 * ApplyGameplayEffectSpecToSelf requires a GameplayEffectSpec. DamageEffectSpecHandle is an effect spec handle.
			 * Now remember the handle itself has an internal DamageEffectSpecHandle.Data and Data itself is a wrapper we have to call DamageEffectSpecHandle.Data.Get() and
			 * all of this returns a pointer which we have to dereference with the star.
			 */
			//TargetAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data.Get(), TargetAbilitySystemComponent);
			TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		
		Destroy();
	}
	else
	{
		bCollisionHit = true;
	}
}

