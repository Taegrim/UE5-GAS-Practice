#include "Characters/CC_EnemyCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/CC_AbilitySystemComponent.h"
#include "AbilitySystem/CC_AttributeSet.h"
#include "AIController.h"
#include "GameplayTags/CCTags.h"
#include "Net/UnrealNetwork.h"


ACC_EnemyCharacter::ACC_EnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = false;
    AbilitySystemComponent = CreateDefaultSubobject<UCC_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

    AttributeSet = CreateDefaultSubobject<UCC_AttributeSet>(TEXT("AttributeSet"));
}

void ACC_EnemyCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, bIsBeingLaunched);
}

UAbilitySystemComponent* ACC_EnemyCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

UAttributeSet* ACC_EnemyCharacter::GetAttributeSet() const
{
    return AttributeSet;
}

void ACC_EnemyCharacter::StopMovementUntilLanded()
{
    bIsBeingLaunched = true;

    AAIController* AIController = GetController<AAIController>();
    if (!IsValid(AIController)) return;

    AIController->StopMovement();

    if (!LandedDelegate.IsAlreadyBound(this, &ThisClass::EnableMovementOnLanded))
    {
        LandedDelegate.AddDynamic(this, &ThisClass::EnableMovementOnLanded);
    }
}

void ACC_EnemyCharacter::EnableMovementOnLanded(const FHitResult& Hit)
{
    bIsBeingLaunched = false;
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, CCTags::Events::Enemy::EndAttack, FGameplayEventData());
    LandedDelegate.RemoveAll(this);
}

void ACC_EnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (!IsValid(GetAbilitySystemComponent())) return;

    // 적 캐릭터는 자기 자신이 ASC를 소유함
    GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
    OnASCInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());

    if (!HasAuthority()) return;

    GiveStartupAbilities();
    InitializeAttributes();

    UCC_AttributeSet* CC_AttributeSet = Cast<UCC_AttributeSet>(GetAttributeSet());
    if (!IsValid(CC_AttributeSet)) return;

    // Attribute를 가져와서  HealthAttribute가 변할때 OnHealthChanged 를 바인딩 하기
    GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(CC_AttributeSet->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
}

void ACC_EnemyCharacter::HandleDeath()
{
    Super::HandleDeath();

    AAIController* AIController = GetController<AAIController>();
    if (!IsValid(AIController)) return;

    AIController->StopMovement();
}
