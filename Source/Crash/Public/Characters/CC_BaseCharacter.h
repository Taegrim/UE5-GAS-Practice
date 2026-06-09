#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "CC_BaseCharacter.generated.h"

class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FASCInitialized, UAbilitySystemComponent*, ASC, UAttributeSet*, AS);

UCLASS(Abstract)
class CRASH_API ACC_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACC_BaseCharacter();

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    virtual UAttributeSet* GetAttributeSet() const;

public:
    // 위젯 초기화 시점에서 ASC가 준비되지 않을 수 있기 때문에 초기화 시점을 알리기 위한 델리게이트
    UPROPERTY(BlueprintAssignable)
    FASCInitialized OnASCInitialized;

protected:
    void GiveStartupAbilities();

    void InitializeAttributes() const;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Crash|Abilities")
    TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

    UPROPERTY(EditDefaultsOnly, Category = "Crash|Abilities")
    TSubclassOf<UGameplayEffect> InitializeAttributesEffect;
};
