// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "CC_AttributeChangeTask.generated.h"

struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, FGameplayAttribute, Attribute, float, NewValue,
                                               float, OldValue);

UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class CRASH_API UCC_AttributeChangeTask : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
    static UCC_AttributeChangeTask* ListenForAttributeChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute Attribute);

    UFUNCTION(BlueprintCallable)
    void EndTask();

    void AttributeChanged(const FOnAttributeChangeData& Data);

public:
    UPROPERTY(BlueprintAssignable)
    FOnAttributeChanged OnAttributeChanged;

    TWeakObjectPtr<UAbilitySystemComponent> ASC;

    FGameplayAttribute AttributeToListenFor;
};
