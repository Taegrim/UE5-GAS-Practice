#include "Crash/Public/Characters/CC_BaseCharacter.h"

#include "AbilitySystemComponent.h"


ACC_BaseCharacter::ACC_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

    // 렌더링 여부와 관계 없이 뼈대는 데디케이트 서버에서 업데이트 됨
    // 애니메이션에서 히트박스를 트리거하면 서버에서 업데이트 됨
    GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

UAbilitySystemComponent* ACC_BaseCharacter::GetAbilitySystemComponent() const
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
