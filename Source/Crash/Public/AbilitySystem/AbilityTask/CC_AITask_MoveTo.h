// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tasks/AITask_MoveTo.h"
#include "CC_AITask_MoveTo.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIMoveTaskCompletedSignature, TEnumAsByte<EPathFollowingResult::Type>, Result, AAIController*, AIController);

UCLASS()
class CRASH_API UCC_AITask_MoveTo : public UAITask_MoveTo
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "AI|Tasks", meta = (AdvancedDisplay = "AcceptanceRadius,StopOnOverlap,AcceptPartialPath,bUsePathfinding,bUseContinuousGoalTracking,ProjectGoalOnNavigation,RequireNavigableEndLocation", DefaultToSelf = "Controller", BlueprintInternalUseOnly = "TRUE", DisplayName = "Move To Location or Actor"))
    static UCC_AITask_MoveTo* AIMoveToProxy(AAIController* Controller, FVector GoalLocation, AActor* GoalActor = nullptr,
       float AcceptanceRadius = -1.f, EAIOptionFlag::Type StopOnOverlap = EAIOptionFlag::Default, EAIOptionFlag::Type AcceptPartialPath = EAIOptionFlag::Default,
       bool bUsePathfinding = true, bool bLockAILogic = true, bool bUseContinuousGoalTracking = false, EAIOptionFlag::Type ProjectGoalOnNavigation = EAIOptionFlag::Default,
       EAIOptionFlag::Type RequireNavigableEndLocation = EAIOptionFlag::Default);

    FMoveTaskCompletedSignature* GetOnMoveFinished();
};
