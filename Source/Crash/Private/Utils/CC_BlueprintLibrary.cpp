// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/CC_BlueprintLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/CC_AttributeSet.h"
#include "Characters/CC_BaseCharacter.h"
#include "GameplayTags/CCTags.h"
#include "Kismet/GameplayStatics.h"

EHitDirection UCC_BlueprintLibrary::GetHitDirection(const FVector& TargetForward, const FVector& ToInstigator)
{
    const float Dot = FVector::DotProduct(TargetForward, ToInstigator);

    if (Dot < -0.5f)
    {
        return EHitDirection::Back;
    }
    if (Dot < 0.5f)
    {
        // 왼쪽 or 오른쪽
        const FVector Cross = FVector::CrossProduct(TargetForward, ToInstigator);
        if (Cross.Z < 0.f)
        {
            return EHitDirection::Left;
        }
        return EHitDirection::Right;
    }

    return EHitDirection::Forward;
}

FName UCC_BlueprintLibrary::GetHitDirectionName(const EHitDirection HitDirection)
{
    switch (HitDirection)
    {
        case EHitDirection::Left: return FName("Left");
        case EHitDirection::Right: return FName("Right");
        case EHitDirection::Forward: return FName("Forward");
        case EHitDirection::Back: return FName("Back");
        default: return FName("None");
    }
}

FClosestActorWithTagResult UCC_BlueprintLibrary::FindClosestActorWithTag(const UObject* WorldContextObject,
    const FVector& Origin, const FName& Tag)
{
    TArray<AActor*> ActorsWithTag;
    UGameplayStatics::GetAllActorsWithTag(WorldContextObject, Tag, ActorsWithTag);

    float ClosestDistance = TNumericLimits<float>::Max();
    AActor* ClosestActor = nullptr;

    for (AActor* Actor : ActorsWithTag)
    {
        if (!IsValid(Actor)) continue;

        ACC_BaseCharacter* Character = Cast<ACC_BaseCharacter>(Actor);
        if (!IsValid(Character) || !Character->IsAlive()) continue;

        const float Distance = FVector::Dist(Origin, Actor->GetActorLocation());
        if (Distance < ClosestDistance)
        {
            ClosestDistance = Distance;
            ClosestActor = Actor;
        }
    }

    FClosestActorWithTagResult Result;
    Result.Actor = ClosestActor;
    Result.Distance = IsValid(ClosestActor) ? ClosestDistance : 0.f;

    return Result;
}

void UCC_BlueprintLibrary::SendDamageEventToPlayer(AActor* Target, const TSubclassOf<UGameplayEffect>& DamageEffect,
    const FGameplayEventData& Payload, const FGameplayTag& DataTag, float Damage)
{
    ACC_BaseCharacter* PlayerCharacter = Cast<ACC_BaseCharacter>(Target);
    if (!IsValid(PlayerCharacter)) return;
    if (!PlayerCharacter->IsAlive()) return;

    UCC_AttributeSet* AttributeSet = Cast<UCC_AttributeSet>(PlayerCharacter->GetAttributeSet());
    if (!IsValid(AttributeSet)) return;

    const bool bLethal = AttributeSet->GetHealth() - Damage <= 0.f;
    const FGameplayTag Tag = bLethal ? CCTags::Events::Player::Death :CCTags::Events::Player::HitReact;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(PlayerCharacter, Tag, Payload);


    UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
    if (!IsValid(ASC)) return;

    FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
    FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DamageEffect, 1.f, ContextHandle);

    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DataTag, -Damage);

    ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}
