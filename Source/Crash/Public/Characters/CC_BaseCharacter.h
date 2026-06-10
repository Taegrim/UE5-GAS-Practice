#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "CC_BaseCharacter.generated.h"

namespace CrashTags
{
    extern CRASH_API const FName Player;
}

class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FASCInitialized, UAbilitySystemComponent*, ASC, UAttributeSet*, AS);

UCLASS(Abstract)
class CRASH_API ACC_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACC_BaseCharacter();

    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    virtual UAttributeSet* GetAttributeSet() const;

    bool IsAlive() const { return bAlive; }
    void SetAlive(bool bAliveState) { bAlive = bAliveState; }

    UFUNCTION(BlueprintCallable, Category = "Crash|Death")
    virtual void HandleRespawn();

    UFUNCTION(BlueprintCallable, Category = "Crash|Attribute")
    void ResetAttribute();

public:
    // 위젯 초기화 시점에서 ASC가 준비되지 않을 수 있기 때문에 초기화 시점을 알리기 위한 델리게이트
    UPROPERTY(BlueprintAssignable)
    FASCInitialized OnASCInitialized;

protected:
    void GiveStartupAbilities();

    void InitializeAttributes() const;

    void OnHealthChanged(const FOnAttributeChangeData& AttributeChangeData);

    virtual void HandleDeath();

private:
    void ApplyGameplayEffect(const TSubclassOf<UGameplayEffect> GameplayEffect, float Level) const;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Crash|Abilities")
    TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

    UPROPERTY(EditDefaultsOnly, Category = "Crash|Abilities")
    TSubclassOf<UGameplayEffect> InitializeAttributesEffect;

    UPROPERTY(EditDefaultsOnly, Category = "Crash|Abilities")
    TSubclassOf<UGameplayEffect> ResetAttributesEffect;

    UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated)
    bool bAlive = true;
};
