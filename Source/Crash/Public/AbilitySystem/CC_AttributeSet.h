// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CC_AttributeSet.generated.h"

// GetAttribute, Getter, Setter, Init 함수를 만들어주는 매크로
// GetAttribute - 이 Attribute가 어떤 클래스의 어떤 프로퍼티인지 나타내는 FGameplayAttribute 반환함
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttributesInitialized);

UCLASS()
class CRASH_API UCC_AttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

public:
    UPROPERTY(BlueprintAssignable)
    FAttributesInitialized OnAttributesInitialized;

    UPROPERTY(ReplicatedUsing = OnRep_AttributesInitialized)
    bool bAttributesInitialize = false;

    UFUNCTION()
    void OnRep_AttributesInitialized();


    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health)
    FGameplayAttributeData Health;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth)
    FGameplayAttributeData MaxHealth;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana)
    FGameplayAttributeData Mana;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana)
    FGameplayAttributeData MaxMana;

    UFUNCTION()
    void OnRep_Health(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    void OnRep_Mana(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    void OnRep_MaxMana(const FGameplayAttributeData& OldValue);

    ATTRIBUTE_ACCESSORS(ThisClass, Health);
    ATTRIBUTE_ACCESSORS(ThisClass, MaxHealth);
    ATTRIBUTE_ACCESSORS(ThisClass, Mana);
    ATTRIBUTE_ACCESSORS(ThisClass, MaxMana);
};
