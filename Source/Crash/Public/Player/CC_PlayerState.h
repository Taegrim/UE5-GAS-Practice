#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "CC_PlayerState.generated.h"

class UAttributeSet;
class UCC_AbilitySystemComponent;

UCLASS()
class CRASH_API ACC_PlayerState : public APlayerState, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    ACC_PlayerState();

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    UAttributeSet* GetAttributeSet() const { return AttributeSet; }

private:
    UPROPERTY(VisibleAnywhere, Category = "Crash|Abilities")
    TObjectPtr<UCC_AbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<UAttributeSet> AttributeSet;
};
