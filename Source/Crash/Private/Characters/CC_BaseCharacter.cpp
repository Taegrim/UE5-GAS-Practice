#include "Crash/Public/Characters/CC_BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

namespace CrashTags
{
    const FName Player = FName("Player");
}

ACC_BaseCharacter::ACC_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

    // 렌더링 여부와 관계 없이 뼈대는 데디케이트 서버에서 업데이트 됨
    // 애니메이션에서 히트박스를 트리거하면 서버에서 업데이트 됨
    GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

void ACC_BaseCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, bAlive);
}

UAbilitySystemComponent* ACC_BaseCharacter::GetAbilitySystemComponent() const
{
    return nullptr;
}

UAttributeSet* ACC_BaseCharacter::GetAttributeSet() const
{
    return nullptr;
}

void ACC_BaseCharacter::GiveStartupAbilities()
{
    if (!IsValid(GetAbilitySystemComponent())) return;

    for (const TSubclassOf<UGameplayAbility>& Ability : StartupAbilities)
    {
        // 어떤 UGameplayAbility 인지, 어빌리티의 레벨, 어떤 입력에 연결할건지, 핸들 등을 전달함
        // 주로 Tag, EnhancedInput 등을 사용하므로 InputID 를 사용하진 않음
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
        GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
    }
}

void ACC_BaseCharacter::InitializeAttributes() const
{
    ApplyGameplayEffect(InitializeAttributesEffect, 1.f);
}

void ACC_BaseCharacter::OnHealthChanged(const FOnAttributeChangeData& AttributeChangeData)
{
    if (AttributeChangeData.NewValue <= 0.f)
    {
        HandleDeath();
    }
}

void ACC_BaseCharacter::HandleDeath()
{
    bAlive = false;

    if (IsValid(GEngine))
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("%s Died"), *GetName()));
    }
}

void ACC_BaseCharacter::ApplyGameplayEffect(const TSubclassOf<UGameplayEffect> GameplayEffect, float Level) const
{
    checkf(IsValid(GameplayEffect), TEXT("%s AttributesEffect not set"), *GameplayEffect.Get()->GetName());

    FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
    FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffect, Level, ContextHandle);

    GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void ACC_BaseCharacter::HandleRespawn()
{
    bAlive = true;
}

void ACC_BaseCharacter::ResetAttribute()
{
    ApplyGameplayEffect(ResetAttributesEffect, 1.f);
}
