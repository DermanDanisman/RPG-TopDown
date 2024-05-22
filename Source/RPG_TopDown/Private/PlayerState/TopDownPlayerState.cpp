// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/TopDownPlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"

ATopDownPlayerState::ATopDownPlayerState()
{
    // Sets the frequency at which this actor's network updates are sent to the clients.
    /** This sets the frequency at which this actor's network updates are sent to the clients.
     * Higher values mean more frequent updates, which can improve the accuracy of the data at the cost of increased network traffic.
     * A value of 100 is quite high, which means updates are sent frequently.
     */
    NetUpdateFrequency = 100.f;

    // Create and initialize the AbilitySystemComponent
    AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>("AbilitySystemComponent");
    // Set the AbilitySystemComponent to be replicated
    AbilitySystemComponent->SetIsReplicated(true);
    // Set the replication mode to Mixed
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	/** ### `SetReplicationMode` and `EGameplayEffectReplicationMode`

    In Unreal Engine, the `SetReplicationMode` function and `EGameplayEffectReplicationMode` enumeration are used within the Gameplay Ability System (GAS)
    to control how gameplay effects are replicated over the network. This can have significant impacts on network performance and gameplay accuracy.

    ### `SetReplicationMode`

    This function sets the replication mode for a gameplay effect, determining how it is synchronized between the server and clients.
    This is crucial in multiplayer games to ensure that all players have a consistent view of gameplay effects while balancing network performance.

    ### `EGameplayEffectReplicationMode`

    `EGameplayEffectReplicationMode` is an enumeration that defines different modes of replication for gameplay effects.
    Here are the different modes along with real-world examples of their usages:

    1. **Minimal (EGameplayEffectReplicationMode::Minimal)**

        - **Description**: Only the essential data needed to keep clients updated is replicated.
        This mode reduces network traffic by minimizing the amount of data sent.
        
        - **Usage Example**: 
            - **Game Type**: Large-scale multiplayer games, like MMOs.
            - **Scenario**: When a player is under the influence of a buff that slightly increases their health regeneration over time,
            the exact regeneration rate might not need to be frequently updated for all clients. The effect can be tracked on the server,
            and only essential updates (like effect start and end) are sent to clients.

        - **Benefits**: 
            - Reduces network bandwidth usage.
            - Improves performance for games with many concurrent effects.

        - **Drawbacks**: 
            - Less precise information on clients, which might lead to inconsistencies in displaying effect details.

    2. **Mixed (EGameplayEffectReplicationMode::Mixed)**

        - **Description**: A balance between minimal and full replication. Some data is sent frequently while other data is sent less often.
        
        - **Usage Example**: 
            - **Game Type**: Action RPGs with a moderate number of active effects.
            - **Scenario**: A character under a complex debuff might have frequent updates for critical information (like health reduction per second)
            while other data (like a slight movement speed decrease) is updated less frequently.

        - **Benefits**: 
            - Balances between network performance and data accuracy.
            - Ensures that critical gameplay information is kept up-to-date.

        - **Drawbacks**: 
            - More complex to implement as it requires deciding which data is critical and which is not.
            - Still uses more bandwidth than minimal mode but less than full mode.

    3. **Full (EGameplayEffectReplicationMode::Full)**

        - **Description**: All data related to the gameplay effect is fully replicated.
        This ensures that all clients have the most accurate and up-to-date information.
        
        - **Usage Example**: 
            - **Game Type**: Competitive multiplayer shooters or real-time strategy games.
            - **Scenario**: A character receiving a powerful shield buff that significantly alters gameplay.
            All clients need to be aware of the exact shield value, duration, and other modifiers to ensure fair play
            and accurate hit detection.

        - **Benefits**: 
            - Ensures complete accuracy and consistency across all clients.
            - Critical for effects that significantly impact gameplay.

        - **Drawbacks**: 
            - High network bandwidth usage.
            - Can lead to performance issues in games with many active effects.

    ### Real-World Examples
    
    1. **Minimal Replication Example**:
        - **Game**: World of Warcraft (WoW).
        - **Scenario**: Passive buffs or debuffs that slightly alter a player's stats over time.
        For example, a small agility increase from a buff that doesn’t drastically impact immediate gameplay.
        The effect's presence is known, but detailed stat changes are handled on the server with minimal client updates.

    2. **Mixed Replication Example**:
        - **Game**: Destiny 2.
        - **Scenario**: A player activates a special ability that temporarily increases their damage output and
        alters their movement speed. The critical damage boost is frequently updated,
        while the movement speed change is updated less often to balance network load.

    3. **Full Replication Example**:
        - **Game**: Overwatch.
        - **Scenario**: A healer character applies a powerful healing over time effect to a teammate.
        The healing effect is critical and needs to be accurately represented on all clients to ensure fair play and
        proper hit detection. Full replication ensures all players see the same health values and effect durations.

    ### Conclusion
    
    Choosing the appropriate `EGameplayEffectReplicationMode` depends on the specific needs of the game and
    the particular gameplay effects being used. Balancing network performance and
    gameplay accuracy is crucial for providing a smooth and fair multiplayer experience. By understanding and
    utilizing the different replication modes, developers can optimize their games to run efficiently
    while maintaining the necessary level of detail and consistency for gameplay effects.
    */
    
    // Create and initialize the AttributeSet
    AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");
}


// Get the Ability System Component associated with this player state
//@return Pointer to the UAbilitySystemComponent
UAbilitySystemComponent* ATopDownPlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

//Get the Attribute Set associated with this player state
//@return Pointer to the UAttributeSet
UAttributeSet* ATopDownPlayerState::GetAttributeSet() const
{
    return AttributeSet;
}
