#include "Crash/Public/Player/CC_PlayerState.h"

#include "AbilitySystemComponent.h"

ACC_PlayerState::ACC_PlayerState()
{
    SetNetUpdateFrequency(100.f);

    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
    // Mixed -> 소유자에겐 자세하게, 다른 클라이언트에겐 최소한만 복제함
    // 캐릭터나 플레이어 스테이트에 사용하기에 적합함


}

UAbilitySystemComponent* ACC_PlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}
