#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CC_GameplayAbility.h"
#include "CC_Primary.generated.h"

UCLASS()
class CRASH_API UCC_Primary : public UCC_GameplayAbility
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Crash|Abilities")
    void SendHitReactEventToActors(const TArray<AActor*>& HitActors);

private:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crash|Abilities", meta = (AllowPrivateAccess = true))
    float HitBoxRadius = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crash|Abilities", meta = (AllowPrivateAccess = true))
    float HitBoxForwardOffset = 200.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crash|Abilities", meta = (AllowPrivateAccess = true))
    float HitBoxElevationOffset = 20.f;
};
