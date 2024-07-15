#pragma once

#include "GameplayEffectTypes.h"
#include "TopDownCustomAbilityTypes.generated.h"

// Declare a custom struct inheriting from FGameplayEffectContext
USTRUCT(BlueprintType)
struct FTopDownGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	// Getter for evasion status
	bool IsEvaded() const { return bIsEvaded; }
	// Getter for critical hit status
	bool IsCriticalHit() const { return bIsCriticalHit; }
    
	// Setter for evasion status
	void SetIsEvaded(bool bInIsEvaded) { bIsEvaded = bInIsEvaded; }
	// Setter for critical hit status
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FTopDownGameplayEffectContext* Duplicate() const override
	{
		FTopDownGameplayEffectContext* NewContext = new FTopDownGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/**
	 * Custom serialization, subclasses must override this
	 * NetSerialize determines how this struct is serialized. Now serialization is necessary if you're going to save one of these structs, for example, or send it across the network.
	 * These are things that require serialization. In other words, take a struct, convert it into bits, zeros and ones and send those bits in a stream across the internet.
	 * If you want to be able to replicate something, it needs this net serialize.
	 */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

protected:

	// Property to store evasion status
	UPROPERTY()
	bool bIsEvaded = false;
    
	// Property to store critical hit status
	UPROPERTY()
	bool bIsCriticalHit = false;
	
};

// Custom serialization traits for FTopDownGameplayEffectContext
template<>
struct TStructOpsTypeTraits<FTopDownGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FTopDownGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};