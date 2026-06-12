#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Components/WidgetComponent.h"
#include "CC_WidgetComponent.generated.h"


class UAbilitySystemComponent;
class UCC_AttributeSet;
class UCC_AbilitySystemComponent;
class ACC_BaseCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRASH_API UCC_WidgetComponent : public UWidgetComponent
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

    // 현재값 Attribute 와 최대값 Attribute의 쌍을 지정하는 맵
    UPROPERTY(EditAnywhere)
    TMap<FGameplayAttribute, FGameplayAttribute> AttributeMap;
    
private:
    void InitAbilitySystemData();

    bool IsASCInitialized() const;

    void InitializeAttributeDelegate();

    UFUNCTION()
    void OnASCInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AS);

    UFUNCTION()
    void BindToAttributeChanges();

    void BindWidgetToAttributeChanges(UWidget* WidgetObject, const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const;

private:
    TWeakObjectPtr<ACC_BaseCharacter> CrashCharacter;
    TWeakObjectPtr<UCC_AbilitySystemComponent> AbilitySystemComponent;
    TWeakObjectPtr<UCC_AttributeSet> AttributeSet;
};
