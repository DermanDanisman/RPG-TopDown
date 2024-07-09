// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TopDownProjectileAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/TopDownProjectile.h"
#include "Interface/Interaction/CombatInterface.h"
#include "TopDownGameplayTags.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"


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
		// Get the ability system component.
		const UAbilitySystemComponent* SourceAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		// Get the attribute set containing the SpellPower attribute.
		const UBaseAttributeSet* BaseAttributeSet = Cast<UBaseAttributeSet>(SourceAbilitySystemComponent->GetAttributeSet(BaseAttributeSetClass));
        
		// Create the gameplay effect spec handle for the damage effect.
		const FGameplayEffectSpecHandle EffectSpecHandle = SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceAbilitySystemComponent->MakeEffectContext());

		FTopDownGameplayTags GameplayTags = FTopDownGameplayTags::Get();
		// Get the current value of the SpellPower attribute
		bool bFound;
		const float ScaledDamage = SourceAbilitySystemComponent->GetGameplayAttributeValue(BaseAttributeSet->GetSpellPowerAttribute(), bFound) * Damage.GetValueAtLevel(1);
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("FireBolt Damage: %f"), ScaledDamage));
		// This adds include. and does the same thing as EffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(GameplayTags.Damage, 50.f);
		//UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Damage, 50.f);
		// Set the damage value in the effect spec handle.
		EffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(GameplayTags.Damage, ScaledDamage);

		// Assign the damage effect spec handle to the projectile.
		Projectile->DamageEffectSpecHandle = EffectSpecHandle;

		// Finalize the spawning process for the projectile.
		Projectile->FinishSpawning(SpawnTransform);
	}
}
