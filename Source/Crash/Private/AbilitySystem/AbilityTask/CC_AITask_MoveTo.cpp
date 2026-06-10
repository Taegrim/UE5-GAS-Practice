// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/CC_AITask_MoveTo.h"
#include "AIController.h"

UCC_AITask_MoveTo* UCC_AITask_MoveTo::AIMoveToProxy(AAIController* Controller, FVector InGoalLocation, AActor* InGoalActor,
                                                  float AcceptanceRadius, EAIOptionFlag::Type StopOnOverlap, EAIOptionFlag::Type AcceptPartialPath,
                                                  bool bUsePathfinding, bool bLockAILogic, bool bUseContinuousGoalTracking,
                                                  EAIOptionFlag::Type ProjectGoalOnNavigation, EAIOptionFlag::Type RequireNavigableEndLocation)
{
    UCC_AITask_MoveTo* MyTask = Controller ? UAITask::NewAITask<UCC_AITask_MoveTo>(*Controller, EAITaskPriority::High) : nullptr;
    if (MyTask)
    {
        FAIMoveRequest MoveReq;
        if (InGoalActor)
        {
            MoveReq.SetGoalActor(InGoalActor);
        }
        else
        {
            MoveReq.SetGoalLocation(InGoalLocation);
        }

        MoveReq.SetAcceptanceRadius(AcceptanceRadius);
        MoveReq.SetReachTestIncludesAgentRadius(FAISystem::PickAIOption(StopOnOverlap, MoveReq.IsReachTestIncludingAgentRadius()));
        MoveReq.SetAllowPartialPath(FAISystem::PickAIOption(AcceptPartialPath, MoveReq.IsUsingPartialPaths()));
        MoveReq.SetUsePathfinding(bUsePathfinding);
        MoveReq.SetProjectGoalLocation(FAISystem::PickAIOption(ProjectGoalOnNavigation, MoveReq.IsProjectingGoal()));
        MoveReq.SetRequireNavigableEndLocation(FAISystem::PickAIOption(RequireNavigableEndLocation, MoveReq.IsNavigableEndLocationRequired()));
        if (Controller)
        {
            MoveReq.SetNavigationFilter(Controller->GetDefaultNavigationFilterClass());
        }

        MyTask->SetUp(Controller, MoveReq);
        MyTask->SetContinuousGoalTracking(bUseContinuousGoalTracking);

        if (bLockAILogic)
        {
            MyTask->RequestAILogicLocking();
        }
    }

    return MyTask;
}

FMoveTaskCompletedSignature* UCC_AITask_MoveTo::GetOnMoveFinished()
{
    return &OnMoveFinished;
}
