#include "AbilitySystem/Abilities/Player/CC_Primary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTags/CCTags.h"

void UCC_Primary::SendHitReactEventToActors(const TArray<AActor*>& HitActors)
{

    for (AActor* HitActor : HitActors)
    {
        FGameplayEventData Payload;
        Payload.Instigator = GetAvatarActorFromActorInfo();

        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, CCTags::Events::Enemy::HitReact, Payload);
    }
}
