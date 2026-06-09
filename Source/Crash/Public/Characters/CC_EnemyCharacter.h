#pragma once

#include "CoreMinimal.h"
#include "CC_BaseCharacter.h"
#include "CC_EnemyCharacter.generated.h"

class UCC_AbilitySystemComponent;

UCLASS()
class CRASH_API ACC_EnemyCharacter : public ACC_BaseCharacter
{
    GENERATED_BODY()

public:
    ACC_EnemyCharacter();

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCC_AbilitySystemComponent> AbilitySystemComponent;
};
