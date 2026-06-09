#include "Crash/Public/Player/CC_PlayerState.h"

#include "AbilitySystem/CC_AbilitySystemComponent.h"
#include "AbilitySystem/CC_AttributeSet.h"

ACC_PlayerState::ACC_PlayerState()
{
    SetNetUpdateFrequency(100.f);

    AbilitySystemComponent = CreateDefaultSubobject<UCC_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
    // Mixed -> 소유자에겐 자세하게, 다른 클라이언트에겐 최소한만 복제함
    // 캐릭터나 플레이어 스테이트에 사용하기에 적합함

    AttributeSet = CreateDefaultSubobject<UCC_AttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ACC_PlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}
