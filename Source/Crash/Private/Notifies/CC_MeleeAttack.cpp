// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CC_MeleeAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "KismetTraceUtils.h"
#include "Abilities/GameplayAbility.h"
#include "Characters/CC_PlayerCharacter.h"
#include "GameplayTags/CCTags.h"
#include "Kismet/KismetMathLibrary.h"

void UCC_MeleeAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
                                 const FAnimNotifyEventReference& EventReference)
{
    if (!IsValid(MeshComp)) return;
    if (!IsValid(MeshComp->GetOwner())) return;

    TArray<FHitResult> HitResults = PerformSphereTrace(MeshComp);
    SendEventToActor(HitResults, MeshComp);
}

TArray<FHitResult> UCC_MeleeAttack::PerformSphereTrace(USkeletalMeshComponent* MeshComp) const
{
    FTransform SocketTransform = MeshComp->GetSocketTransform(SocketName);

    const FVector Start = SocketTransform.GetLocation();
    const FVector ExtendedSocketDirection = UKismetMathLibrary::GetForwardVector(SocketTransform.GetRotation().Rotator()) * SocketExtensionOffset;
    //const FVector ExtendedSocketDirection = SocketTransform.GetRotation().GetForwardVector() * SocketExtensionOffset;
    const FVector End = Start - ExtendedSocketDirection;

    TArray<FHitResult> OutHits;

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(MeshComp->GetOwner());

    FCollisionResponseParams ResponseParams;
    ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
    ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);

    UWorld* World = GEngine->GetWorldFromContextObject(MeshComp, EGetWorldErrorMode::LogAndReturnNull);
    if (!IsValid(World)) return{};

    bool const bHit = World->SweepMultiByChannel(
        OutHits,
        Start,
        End,
        FQuat::Identity,
        ECC_Visibility,
        FCollisionShape::MakeSphere(SphereTraceRadius),
        QueryParams,
        ResponseParams
        );

    if (bDrawDebugs)
    {
        DrawDebugSphereTraceMulti(
            World,
            Start,
            End,
            SphereTraceRadius,
            EDrawDebugTrace::ForDuration,
            bHit,
            OutHits,
            FColor::Red,
            FColor::Green,
            5.f
            );
    }


    return OutHits;
}

void UCC_MeleeAttack::SendEventToActor(const TArray<FHitResult>& Hits, USkeletalMeshComponent* MeshComp) const
{
    for (const FHitResult& HitResult : Hits)
    {
        ACC_PlayerCharacter* PlayerCharacter = Cast<ACC_PlayerCharacter>(HitResult.GetActor());
        if (!IsValid(PlayerCharacter)) continue;
        if (!PlayerCharacter->IsAlive()) continue;

        UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
        if (!IsValid(ASC)) continue;

        FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
        ContextHandle.AddHitResult(HitResult);

        FGameplayEventData Payload;
        Payload.Target = PlayerCharacter;
        Payload.Instigator = MeshComp->GetOwner();
        Payload.ContextHandle = ContextHandle;

        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
            MeshComp->GetOwner(),
            CCTags::Events::Enemy::MeleeTraceHit,
            Payload
            );
    }
}
