#include "Characters/CC_EnemyCharacter.h"

#include "AbilitySystem/CC_AbilitySystemComponent.h"


ACC_EnemyCharacter::ACC_EnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = false;
    AbilitySystemComponent = CreateDefaultSubobject<UCC_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

UAbilitySystemComponent* ACC_EnemyCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void ACC_EnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (!IsValid(GetAbilitySystemComponent())) return;

    // 적 캐릭터는 자기 자신이 ASC를 소유함
    GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);

    if (!HasAuthority()) return;

    GiveStartupAbilities();
}
