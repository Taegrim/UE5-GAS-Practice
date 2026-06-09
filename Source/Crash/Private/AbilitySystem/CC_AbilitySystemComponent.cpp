#include "AbilitySystem/CC_AbilitySystemComponent.h"

#include "GameplayTags/CCTags.h"


void UCC_AbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
    Super::OnGiveAbility(AbilitySpec);

    if (!IsValid(AbilitySpec.Ability)) return;

    HandleAutoActivatedAbility(AbilitySpec);
}

void UCC_AbilitySystemComponent::OnRep_ActivateAbilities()
{
    Super::OnRep_ActivateAbilities();

    FScopedAbilityListLock ActiveScopeLock(*this);
    for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        HandleAutoActivatedAbility(AbilitySpec);
    }
}

void UCC_AbilitySystemComponent::SetAbilityLevel(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level)
{
    AActor* Avatar = GetAvatarActor();
    if (!IsValid(Avatar)) return;
    if (!Avatar->HasAuthority()) return;

    if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromClass(AbilityClass))
    {
        AbilitySpec->Level = Level;
        MarkAbilitySpecDirty(*AbilitySpec);
    }
}

void UCC_AbilitySystemComponent::AddToAbilityLevel(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level)
{
    AActor* Avatar = GetAvatarActor();
    if (!IsValid(Avatar)) return;
    if (!Avatar->HasAuthority()) return;

    if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromClass(AbilityClass))
    {
        AbilitySpec->Level += Level;
        MarkAbilitySpecDirty(*AbilitySpec);
    }
}

void UCC_AbilitySystemComponent::HandleAutoActivatedAbility(const FGameplayAbilitySpec& AbilitySpec)
{
    for (const FGameplayTag& Tag : AbilitySpec.Ability->GetAssetTags() )
    {
        if (Tag.MatchesTagExact(CCTags::CCAbilities::ActivateOnGiven))
        {
            TryActivateAbility(AbilitySpec.Handle);
            return;
        }
    }
}

