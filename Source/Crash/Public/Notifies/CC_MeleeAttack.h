// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CC_MeleeAttack.generated.h"


UCLASS()
class CRASH_API UCC_MeleeAttack : public UAnimNotifyState
{
    GENERATED_BODY()

public:
    virtual void NotifyTick(
        USkeletalMeshComponent* MeshComp,
        UAnimSequenceBase* Animation,
        float FrameDeltaTime,
        const FAnimNotifyEventReference& EventReference) override;

private:
    TArray<FHitResult> PerformSphereTrace(USkeletalMeshComponent* MeshComp) const;
    void SendEventToActor(const TArray<FHitResult>& Hits, USkeletalMeshComponent* MeshComp) const;

private:
    UPROPERTY(EditAnywhere, Category = "Crash|Debugs", meta = (AllowPrivateAccess = "true"))
    bool bDrawDebugs = false;

    UPROPERTY(EditAnywhere, Category = "Crash|Socket", meta = (AllowPrivateAccess = "true"))
    FName SocketName{FName(TEXT("FX_Trail_01_R"))};

    UPROPERTY(EditAnywhere, Category = "Crash|Socket", meta = (AllowPrivateAccess = "true"))
    float SocketExtensionOffset{40.f};

    UPROPERTY(EditAnywhere, Category = "Crash|Trace", meta = (AllowPrivateAccess = "true"))
    float SphereTraceRadius{60.f};

};
