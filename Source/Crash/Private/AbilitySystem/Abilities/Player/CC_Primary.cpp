#include "AbilitySystem/Abilities/Player/CC_Primary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Engine/OverlapResult.h"
#include "GameplayTags/CCTags.h"

TArray<AActor*> UCC_Primary::HitBoxOverlapTest()
{
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(GetAvatarActorFromActorInfo());

    // 오버랩 시 아바타 액터가를 무시하도록 함
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActors(ActorsToIgnore);

    // 폰의 Block을 제외한 모든 채널을 무시함
    FCollisionResponseParams ResponseParams;
    ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
    ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);

    TArray<FOverlapResult> OverlapResults;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(HitBoxRadius);

    const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector() * HitBoxForwardOffset;
    const FVector HitBoxLocation = GetAvatarActorFromActorInfo()->GetActorLocation() + Forward + FVector(0.f, 0.f, HitBoxElevationOffset);

    GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        HitBoxLocation,
        FQuat::Identity,
        ECC_Visibility,
        Sphere,
        QueryParams,
        ResponseParams
        );

    TArray<AActor*> HitActors;
    for (const FOverlapResult& Result : OverlapResults)
    {
        if (!IsValid(Result.GetActor())) continue;

        HitActors.AddUnique(Result.GetActor());
    }

    if (bDrawDebugs)
    {
        DrawHitBoxOverlapDebugs(OverlapResults, HitBoxLocation);
    }

    return HitActors;
}

void UCC_Primary::SendHitReactEventToActors(const TArray<AActor*>& HitActors)
{

    for (AActor* HitActor : HitActors)
    {
        FGameplayEventData Payload;
        Payload.Instigator = GetAvatarActorFromActorInfo();

        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, CCTags::Events::Enemy::HitReact, Payload);
    }
}

void UCC_Primary::DrawHitBoxOverlapDebugs(const TArray<FOverlapResult>& OverlapResults, const FVector& HitBoxLocation) const
{
    DrawDebugSphere(
            GetWorld(),
            HitBoxLocation,
            HitBoxRadius,
            16,
            FColor::Red,
            false,
            3.f
            );

    for (const FOverlapResult& Result : OverlapResults)
    {
        if (IsValid(Result.GetActor()))
        {
            FVector DebugLocation = Result.GetActor()->GetActorLocation();
            DebugLocation.Z += 100.f;

            DrawDebugSphere(
                GetWorld(),
                DebugLocation,
                30.f,
                10,
                FColor::Green,
                false,
                3.f
                );
        }
    }
}
