#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "GameplayEffectTypes.h"
#include "APlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWeaponManager;
class UMagicAttributeSet;

UCLASS()
class GASRPG_API AAPlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponComponent", meta = (AllowPrivateAccess = "true"))
	UWeaponManager* WeaponComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayAbility> DashAbility;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMagicAttributeSet> MagicAttributeSet;

	bool ASCInputBound;


	// EFFECTS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AbilitySystem|Effects", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> EnergyRegenEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AbilitySystem|Effects", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> ManaRegenEffectClass;
	
	/* INPUT */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* Ability1InputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* Ability2InputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* Ability3InputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* Ability4InputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* Weapon1InputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* Weapon2InputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* Weapon3InputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ConfirmAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* CancelAction;

public:
	AAPlayerCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Ability1InputTriggered(const FInputActionValue& Value);

	UFUNCTION()
	void Ability2InputTriggered(const FInputActionValue& Value);

	UFUNCTION()
	void Ability3InputTriggered(const FInputActionValue& Value);

	UFUNCTION()
	void Ability4InputTriggered(const FInputActionValue& Value);

	UFUNCTION()
	void Weapon1InputTriggered(const FInputActionValue& Value);

	UFUNCTION()
	void Weapon2InputTriggered(const FInputActionValue& Value);

	UFUNCTION()
	void Weapon3InputTriggered(const FInputActionValue& Value);

	UFUNCTION()
	void OnConfirmPressed(const FInputActionValue& Value);

	UFUNCTION()
	void OnCancelPressed(const FInputActionValue& Value);

	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComponent; }

private:

	void BindASCInput();

	void HandleAttributeRegen(const FGameplayTag GameplayTag, const TSubclassOf<UGameplayEffect> EffectClass, const float NewValue, const float OldValue) const;

	void OnEnergyChanged(const FOnAttributeChangeData& AttributeChangeData);
	void OnManaChanged(const FOnAttributeChangeData& AttributeChangeData);
	void OnMovementSpeedChanged(const FOnAttributeChangeData& AttributeChangeData);
};
