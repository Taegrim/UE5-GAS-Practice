// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CC_AttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameplayTags/CCTags.h"
#include "Net/UnrealNetwork.h"

void UCC_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Mana, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxMana, COND_None, REPNOTIFY_Always);

    DOREPLIFETIME(ThisClass, bAttributesInitialize);
}

// GamePlayEffect가 적용된 후에 호출됨
// 현재는 초기화에 GamePlayEffect 사용해서 초기화 후에 호출됨
void UCC_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
    }
    if (Data.EvaluatedData.Attribute == GetManaAttribute())
    {
        SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
    }

    // GamePlayEffect 적용시 Attribute가 체력이고 0이하로 떨어지면 죽은것
    if (Data.EvaluatedData.Attribute == GetHealthAttribute() && GetHealth() <= 0.f)
    {
        AActor* Killer = Data.EffectSpec.GetEffectContext().GetInstigator();
        AActor* DeadActor = Data.Target.GetAvatarActor();

        FGameplayEventData Payload;
        Payload.Instigator = Killer;
        Payload.Target = DeadActor;

        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
            Killer,
            CCTags::Events::KillScored,
            Payload
            );
    }

    if (!bAttributesInitialize)
    {
        bAttributesInitialize = true;
        OnAttributesInitialized.Broadcast();
    }
}

// 위에서 bAttributesInitialize 값이 바뀌면 클라이언트에서 호출됨
void UCC_AttributeSet::OnRep_AttributesInitialized()
{
    if (bAttributesInitialize)
    {
        OnAttributesInitialized.Broadcast();
    }
}

void UCC_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Health, OldValue);
}

void UCC_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealth, OldValue);
}

void UCC_AttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Mana, OldValue);
}

void UCC_AttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxMana, OldValue);
}
