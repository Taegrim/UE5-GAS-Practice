#include "UI/CC_WidgetComponent.h"

#include "AbilitySystem/CC_AbilitySystemComponent.h"
#include "AbilitySystem/CC_AttributeSet.h"
#include "Blueprint/WidgetTree.h"
#include "Characters/CC_BaseCharacter.h"
#include "UI/CC_AttributeWidget.h"


void UCC_WidgetComponent::BeginPlay()
{
    Super::BeginPlay();

    InitAbilitySystemData();

    if (!IsASCInitialized())
    {
        CrashCharacter->OnASCInitialized.AddDynamic(this, &ThisClass::OnASCInitialized);
        return;
    }

    InitializeAttributeDelegate();
}

void UCC_WidgetComponent::InitAbilitySystemData()
{
    CrashCharacter= Cast<ACC_BaseCharacter>(GetOwner());
    AttributeSet = Cast<UCC_AttributeSet>(CrashCharacter->GetAttributeSet());
    AbilitySystemComponent = Cast<UCC_AbilitySystemComponent>(CrashCharacter->GetAbilitySystemComponent());
}

bool UCC_WidgetComponent::IsASCInitialized() const
{
    return AbilitySystemComponent.IsValid() && AttributeSet.IsValid();
}

// AS 값이 초기화 된걸 보장할 수없으므로 확인하고 초기화 됐다면 바로 처리하고
// 초기화되지 않았다면 델리게이트에 바인딩해서 Attribute가 초기화될때 실행될 수 있게 함
void UCC_WidgetComponent::InitializeAttributeDelegate()
{
    if (!AttributeSet->bAttributesInitialize)
    {
        AttributeSet->OnAttributesInitialized.AddDynamic(this, &ThisClass::BindToAttributeChanges);
    }
    else
    {
        // 이미 초기화됨
        BindToAttributeChanges();
    }
}

void UCC_WidgetComponent::OnASCInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
    AbilitySystemComponent = Cast<UCC_AbilitySystemComponent>(ASC);
    AttributeSet = Cast<UCC_AttributeSet>(AS);

    if (!IsASCInitialized()) return;

    InitializeAttributeDelegate();
}

void UCC_WidgetComponent::BindToAttributeChanges()
{
    for (const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair : AttributeMap)
    {
        // 소유한 위젯 객체에 대해 처리함
        BindWidgetToAttributeChanges(GetUserWidgetObject(), Pair);

        // 그게 아니라면 소유한 위젯의 모든 자식 요소를 순회하고 동일한 일 반복
        GetUserWidgetObject()->WidgetTree->ForEachWidget([this, &Pair](UWidget* ChildWidget)
        {
            BindWidgetToAttributeChanges(ChildWidget, Pair);
        });
    }
}

// Attribute 위젯에 대해서만 Bind 해주는 함수
void UCC_WidgetComponent::BindWidgetToAttributeChanges(UWidget* WidgetObject,
    const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const
{
    UCC_AttributeWidget* AttributeWidget = Cast<UCC_AttributeWidget>(WidgetObject);
    if (!IsValid(AttributeWidget)) return;  // CC Attribute 위젯에 대해서만 처리함
    if (!AttributeWidget->MatchesAttributes(Pair)) return;  // 일치하는 Attribute일때만 처리함

    AttributeWidget->OnAttributeChange(Pair, AttributeSet.Get());   // 초기값을 위한 것

    // 게임중에 값이 변경될때 GAS의 델리게이트에 바인딩해서 처리하도록 함
    // Max값에는 바인딩하지 않음, 최대값이 변하는 게임이면 Pair.Value에도 바인딩 필요함
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Key).AddLambda([this, AttributeWidget, &Pair](const FOnAttributeChangeData& AttributeChangeData)
    {
        AttributeWidget->OnAttributeChange(Pair, AttributeSet.Get());
    });
}
