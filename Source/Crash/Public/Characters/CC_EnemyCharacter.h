#pragma once

#include "CoreMinimal.h"
#include "CC_BaseCharacter.h"
#include "CC_EnemyCharacter.generated.h"

class UAttributeSet;
class UCC_AbilitySystemComponent;

UCLASS()
class CRASH_API ACC_EnemyCharacter : public ACC_BaseCharacter
{
    GENERATED_BODY()

public:
    ACC_EnemyCharacter();

    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    virtual UAttributeSet* GetAttributeSet() const override;

    UFUNCTION(BlueprintImplementableEvent)
    float GetTimeLineLength();

    void StopMovementUntilLanded();

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crash|AI")
    float AcceptanceRadius{500.f};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crash|AI")
    float MinAttackDelay{0.1f};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crash|AI")
    float MaxAttackDelay{0.5f};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
    bool bIsBeingLaunched{false};

protected:
    virtual void BeginPlay() override;

    virtual void HandleDeath() override;

private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCC_AbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<UAttributeSet> AttributeSet;

    UFUNCTION()
    void EnableMovementOnLanded(const FHitResult& Hit);
};
