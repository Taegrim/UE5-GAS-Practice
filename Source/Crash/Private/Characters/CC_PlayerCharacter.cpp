#include "Crash/Public/Characters/CC_PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/CC_PlayerState.h"

ACC_PlayerCharacter::ACC_PlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    GetCapsuleComponent()->InitCapsuleSize(42.5f, 96.f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
    GetCharacterMovement()->JumpZVelocity = 500.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;


    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 600.f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
}

UAbilitySystemComponent* ACC_PlayerCharacter::GetAbilitySystemComponent() const
{
    ACC_PlayerState* CCPlayerState = GetPlayerState<ACC_PlayerState>();
    if (!IsValid(CCPlayerState)) return nullptr;

    return CCPlayerState->GetAbilitySystemComponent();
}

UAttributeSet* ACC_PlayerCharacter::GetAttributeSet() const
{
    ACC_PlayerState* CCPlayerState = GetPlayerState<ACC_PlayerState>();
    if (!IsValid(CCPlayerState)) return nullptr;

    return CCPlayerState->GetAttributeSet();
}

// 폰이 컨트롤러에 Possess될때 '서버'에서 호출됨
// 이 시점에서 서버는 어빌리티 시스템 컴포넌트를 초기화 함
void ACC_PlayerCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (!IsValid(GetAbilitySystemComponent()) || !HasAuthority()) return;

    GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
    OnASCInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());
    GiveStartupAbilities();
    InitializeAttributes();
}

// 서버에서 PlayerState가 복제되어 들어올때 '클라이언트'에서 호출됨
// 이 시점에서 클라이언트는 어빌리티 시스템 컴포넌트를 초기화 함
void ACC_PlayerCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    if (!IsValid(GetAbilitySystemComponent())) return;

    GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
    OnASCInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());
}
