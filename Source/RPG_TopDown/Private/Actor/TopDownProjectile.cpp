// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/TopDownProjectile.h"

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

	Sphere->IgnoreActorWhenMoving(GetInstigator(), true);

	SetLifeSpan(LifeSpan);
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ATopDownProjectile::OnSphereOverlap);

	if (LoopingEffectSound)
	{
		LoopingEffectAudioComponent = UGameplayStatics::SpawnSoundAttached(LoopingEffectSound, GetRootComponent());
	}
}

void ATopDownProjectile::Destroyed()
{
	if (!bCollisionHit && !HasAuthority())
	{
		if (ImpactSound && ImpactEffect)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator().ZeroRotator);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());

		}
		LoopingEffectAudioComponent->Stop();
	}
	Super::Destroyed();
}

void ATopDownProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ImpactSound && ImpactEffect)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator().ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	
	}
	LoopingEffectAudioComponent->Stop();
	
	if (HasAuthority())
	{
		Destroy();
	}
	else
	{
		bCollisionHit = true;
	}
}

