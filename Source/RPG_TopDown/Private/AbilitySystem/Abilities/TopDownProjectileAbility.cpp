// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TopDownProjectileAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/TopDownProjectile.h"
#include "Interface/Interaction/CombatInterface.h"
#include "TopDownGameplayTags.h"


void UTopDownProjectileAbility::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	/*
	 * I want to spawn the projectile on the server. I don't want to spawn this locally. I want this projectile to be a replicated actor.
	 * And that way if the server spawns it, then the server will be in charge of moving it,
	 * handling its location and all that good stuff, and the clients will just see a replicated version of the projectile.
	 */
	
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetWeaponTipSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		
		Rotation.Pitch = 0.f;
		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		check(ProjectileClass);
		/*
		 * SpawnActorDeferred() is used when you need to spawn an actor but want to defer its complete initialization.
		 * This allows you to modify its properties or components before the initialization is finished.
		 * This is particularly useful for complex setup or when you need to set specific parameters
		 * that are only available after the actor is partially constructed but not fully initialized.
		 */
		ATopDownProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATopDownProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(),
			Cast<APawn>(GetAvatarActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Projectile->SetInstigator(Cast<APawn>(CurrentActorInfo->AvatarActor));
		Projectile->SetOwner(Cast<APawn>(CurrentActorInfo->AvatarActor));

		// Setting our damage gameplay effect on the projectile.
		const UAbilitySystemComponent* SourceAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		const FGameplayEffectSpecHandle EffectSpecHandle = SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceAbilitySystemComponent->MakeEffectContext());

		FTopDownGameplayTags GameplayTags = FTopDownGameplayTags::Get();
		// This adds include. and does the same thing as EffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(GameplayTags.Damage, 50.f);
		//UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Damage, 50.f);
		EffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(GameplayTags.Damage, 50.f);
		Projectile->DamageEffectSpecHandle = EffectSpecHandle;
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}
