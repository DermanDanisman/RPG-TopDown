// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TopDownProjectileAbility.h"

#include "Actor/TopDownProjectile.h"
#include "Interface/Interaction/CombatInterface.h"


void UTopDownProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	/*
	 * I want to spawn the projectile on the server. I don't want to spawn this locally. I want this projectile to be a replicated actor.
	 * And that way if the server spawns it, then the server will be in charge of moving it,
	 * handling its location and all that good stuff, and the clients will just see a replicated version of the projectile.
	 */

	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer) return;
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetWeaponTipSocketLocation();
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		// TODO: Set the Projectile Rotation.

		check(ProjectileClass);
		/*
		 * SpawnActorDeferred() is used when you need to spawn an actor but want to defer its complete initialization.
		 * This allows you to modify its properties or components before the initialization is finished.
		 * This is particularly useful for complex setup or when you need to set specific parameters
		 * that are only available after the actor is partially constructed but not fully initialized.
		 */
		ATopDownProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATopDownProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(),
			Cast<APawn>(GetAvatarActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		// TODO: Give the Projectile a Gameplay Effect Spec for causing Damage.
		Projectile->FinishSpawning(SpawnTransform);
	}
}
