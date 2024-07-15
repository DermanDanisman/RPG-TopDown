#pragma once

#include "GameplayEffectTypes.h"
#include "TopDownCustomAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FTopDownGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	bool IsEvaded() const { return bIsEvaded; }
	bool IsCriticalHit() const { return bIsCriticalHit; }
	
	void SetIsEvaded(bool bInIsEvaded) { bIsEvaded = bInIsEvaded; }
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

	UPROPERTY()
	bool bIsEvaded = false;
	
	UPROPERTY()
	bool bIsCriticalHit = false;
	
};

template<>
struct TStructOpsTypeTraits<FTopDownGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FTopDownGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};