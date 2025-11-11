#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Ability.generated.h"

enum class EGASAbilityInputID : uint8;
class UGameplayAbility;
class UWidgetController;
struct FGameplayAbilitySpec;
class UTextBlock;
class UImage;
class UOverlay;
class USizeBox;

UCLASS()
class GASRPG_API UAbility : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = true), BlueprintReadOnly)
	USizeBox* SizeBox;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = true), BlueprintReadOnly)
	UOverlay* AbilityOverlay;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = true), BlueprintReadOnly)
	UImage* AbilityBackground;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = true), BlueprintReadOnly)
	UImage* AbilityActivatedImage;

	//UPROPERTY(meta = (BindWidget, AllowPrivateAccess = true), BlueprintReadOnly)
	//UTextBlock* AbilityText;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = true), BlueprintReadOnly)
	UOverlay* CooldownOverlay;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = true), BlueprintReadOnly)
	UImage* CooldownBackground;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = true), BlueprintReadOnly)
	UTextBlock* CooldownText;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = true), BlueprintReadOnly)
	UTextBlock* AbilityKeyBind;
	
	UPROPERTY()
	UWidgetController* WidgetController;
	
	UPROPERTY()
	UGameplayAbility* GameplayAbility;

	FGameplayTag AbilityCooldownTag;

	FTimerHandle CooldownTimer;
	float CooldownDecreaseRate;
	float CooldownTimeRemaining;
	float CooldownDuration;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UMaterialInstanceDynamic* CooldownMID;

public:
	void InitializeFromSpec(const FGameplayAbilitySpec& Spec, UWidgetController* InWidgetController);

private:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCooldownStarted(FGameplayTag GameplayTag, const float& Duration, const float& TimeRemaining);

	UFUNCTION()
	void OnCooldownEnded(FGameplayTag GameplayTag);

	UFUNCTION()
	void OnAbilityActivation(UGameplayAbility* ActivationAbility);

	UFUNCTION()
	void OnAbilityEnded(UGameplayAbility* Ability);
	
	void UpdateCooldownTimer();

	FText GetInputTextFromInputID(EGASAbilityInputID InputID);

};
