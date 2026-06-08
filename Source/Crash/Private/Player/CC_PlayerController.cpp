#include "Crash/Public/Player/CC_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/Character.h"

void ACC_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (!IsValid(InputSubsystem)) return;

    for (UInputMappingContext* Context : InputMappingContexts)
    {
        InputSubsystem->AddMappingContext(Context, 0);
    }

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
    if (!IsValid(EnhancedInputComponent)) return;

    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJumping);
    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
    EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
    EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Started, this, &ThisClass::Primary);
}

void ACC_PlayerController::Jump()
{
    if (!IsValid(GetCharacter())) return;

    GetCharacter()->Jump();
}

void ACC_PlayerController::StopJumping()
{
    if (!IsValid(GetCharacter())) return;

    GetCharacter()->StopJumping();
}

void ACC_PlayerController::Move(const FInputActionValue& Value)
{
    if (!IsValid(GetCharacter())) return;

    const FVector2D MovementVector = Value.Get<FVector2D>();

    // 카메라 회전 기준으로 앞 방향을 찾기
    const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    GetCharacter()->AddMovementInput(ForwardDirection, MovementVector.Y);
    GetCharacter()->AddMovementInput(RightDirection, MovementVector.X);
}

void ACC_PlayerController::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisVector = Value.Get<FVector2D>();

    AddYawInput(LookAxisVector.X);
    AddPitchInput(LookAxisVector.Y);
}

void ACC_PlayerController::Primary()
{
    UE_LOG(LogTemp, Warning, TEXT("Primary Action!"));
}
