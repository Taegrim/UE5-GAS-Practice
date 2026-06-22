// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Enemy/CC_SearchForTarget.h"

#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "AbilitySystem/AbilityTask/CC_AITask_MoveTo.h"
#include "AbilitySystem/AbilityTask/CC_WaitGameplayEvent.h"
#include "Characters/CC_EnemyCharacter.h"
#include "GameplayTags/CCTags.h"
#include "Tasks/AITask_MoveTo.h"
#include "Utils/CC_BlueprintLibrary.h"

UCC_SearchForTarget::UCC_SearchForTarget()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UCC_SearchForTarget::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    OwningEnemy = Cast<ACC_EnemyCharacter>(GetAvatarActorFromActorInfo());
    check(OwningEnemy.IsValid());

    OwningAIController = Cast<AAIController>(OwningEnemy->GetController());
    check(OwningAIController.IsValid());

    StartSearch();

    WaitGameplayEventTask = UCC_WaitGameplayEvent::WaitGameplayEventToActorProxy(
        GetAvatarActorFromActorInfo(),
        CCTags::Events::Enemy::EndAttack
        );
    WaitGameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::EndAttackEventReceived);
    WaitGameplayEventTask->StartActivation();
}

void UCC_SearchForTarget::StartSearch()
{
    if (bDrawDebugs)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                3.f,
                FColor::Red,
                FString::Printf(TEXT("UCC_SearchForTarget::StartSearch"))
                );
        }
    }

    if (!OwningEnemy.IsValid()) return;

    // WaitDelay 공격 대기 시간만큼 WaitDelay 후에 Search 하도록 바인딩, Activate
    const float SearchDelay = FMath::RandRange(OwningEnemy->MinAttackDelay, OwningEnemy->MaxAttackDelay);
    SearchDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, SearchDelay);
    SearchDelayTask->OnFinish.AddDynamic(this, &ThisClass::Search);
    SearchDelayTask->Activate();
}

void UCC_SearchForTarget::EndAttackEventReceived(FGameplayEventData Payload)
{
    if (OwningEnemy.IsValid() && !OwningEnemy->bIsBeingLaunched)
    {
        StartSearch();
    }
}

void UCC_SearchForTarget::Search()
{
    const FVector SearchOrigin = GetAvatarActorFromActorInfo()->GetActorLocation();
    FClosestActorWithTagResult Result = UCC_BlueprintLibrary::FindClosestActorWithTag(
        this,
        SearchOrigin,
        CrashTags::Player
        );

    TargetCharacter = Cast<ACC_BaseCharacter>(Result.Actor);

    if (!TargetCharacter.IsValid())
    {
        StartSearch();
        return;
    }
    if (TargetCharacter->IsAlive())
    {
        MoveToTargetAndAttack();
    }
    else
    {
        StartSearch();
    }
}

void UCC_SearchForTarget::MoveToTargetAndAttack()
{
    if (!OwningEnemy.IsValid() || !OwningAIController.IsValid() || !TargetCharacter.IsValid()) return;

    if (!OwningEnemy->IsAlive())
    {
        StartSearch();
        return;
    }

    MoveToLocationOrActorTask = UCC_AITask_MoveTo::AIMoveToProxy(
        OwningAIController.Get(),
        FVector(),
        TargetCharacter.Get(),
        OwningEnemy->AcceptanceRadius
        );

    MoveToLocationOrActorTask->GetOnMoveFinished()->AddDynamic(this, &ThisClass::AttackTarget);
    MoveToLocationOrActorTask->ConditionalPerformMove();
}

void UCC_SearchForTarget::AttackTarget(TEnumAsByte<EPathFollowingResult::Type> Result, AAIController* AIController)
{
    if (Result != EPathFollowingResult::Success)
    {
        StartSearch();
        return;
    }

    OwningEnemy->RotateToTarget(TargetCharacter.Get());

    AttackDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, OwningEnemy->GetTimeLineLength());
    AttackDelayTask->OnFinish.AddDynamic(this, &ThisClass::Attack);
    AttackDelayTask->Activate();
}

void UCC_SearchForTarget::Attack()
{
    if (!OwningEnemy->IsAlive())
    {
        StartSearch();
        return;
    }

    const FGameplayTag AttackTag = CCTags::CCAbilities::Enemy::Attack;
    if (!GetAbilitySystemComponentFromActorInfo()->TryActivateAbilitiesByTag(AttackTag.GetSingleTagContainer()))
    {
        StartSearch();
        return;
    }
}
