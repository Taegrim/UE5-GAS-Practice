#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CC_PlayerController.generated.h"


struct FGameplayTag;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class CRASH_API ACC_PlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void SetupInputComponent() override;

private:

    UPROPERTY(EditDefaultsOnly, Category = "Crash|Input")
    TArray<TObjectPtr<UInputMappingContext>> InputMappingContexts;

#pragma region Movement Override
    UPROPERTY(EditDefaultsOnly, Category = "Crash|Input|Movement")
    TObjectPtr<UInputAction> JumpAction;

    UPROPERTY(EditDefaultsOnly, Category = "Crash|Input|Movement")
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditDefaultsOnly, Category = "Crash|Input|Movement")
    TObjectPtr<UInputAction> LookAction;


    void Jump();
    void StopJumping();
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

#pragma endregion

#pragma region Ability Override

private:
    UPROPERTY(EditDefaultsOnly, Category = "Crash|Input|Abilities")
    TObjectPtr<UInputAction> PrimaryAction;

    UPROPERTY(EditDefaultsOnly, Category = "Crash|Input|Abilities")
    TObjectPtr<UInputAction> SecondaryAction;

    UPROPERTY(EditDefaultsOnly, Category = "Crash|Input|Abilities")
    TObjectPtr<UInputAction> TertiaryAction;


    void Primary();
    void Secondary();
    void Tertiary();

    void ActivateAbility(const FGameplayTag& AbilityTag) const;

#pragma endregion

private:
    bool IsAlive() const;

};
