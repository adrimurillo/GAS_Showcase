#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "WidgetController.generated.h"

class UGameplayAbility;
struct FGameplayAbilitySpec;
class UMagicAttributeSet;
class UAbilitySystemComponent;
struct FOnAttributeChangeData;
class UBasicAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthUpdated, float, NewValue, float, MaxValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnergyUpdated, float, NewValue, float, MaxValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCooldownStarted, FGameplayTag, AbilityTag, const float&, Duration, const float&, TimeRemaining);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCooldownEnded, FGameplayTag, AbilityTag);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityActivated, UGameplayAbility*, ActivatedAbility);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityEnded, UGameplayAbility*, ActivatedAbility);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSilenceStarted, const float, Duration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSilenceEnded);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFlashStarted, const float, Duration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFlashEnded);

UCLASS()
class GASRPG_API UWidgetController : public UObject
{
	GENERATED_BODY()

public:
	FOnHealthUpdated OnHealthUpdated;
	FOnEnergyUpdated OnEnergyUpdated;

	FOnCooldownStarted OnCooldownStarted;
	FOnCooldownEnded OnCooldownEnded;

	FOnAbilityActivated OnAbilityActivated;
	FOnAbilityEnded OnAbilityEnded;

	FOnSilenceStarted OnSilenceStarted;
	FOnSilenceEnded OnSilenceEnded;

	FOnFlashStarted OnFlashStarted;
	FOnFlashEnded OnFlashEnded;

public:
	void Initialize(UAbilitySystemComponent* InASC, UBasicAttributeSet* InBasicAttributes, UMagicAttributeSet* InMagicAttributes);
	
	FORCEINLINE UAbilitySystemComponent* GetAbilitySystemComponent() const { return ASC; }
private:
	UPROPERTY()
	UAbilitySystemComponent* ASC;

	UPROPERTY()
	UBasicAttributeSet* BasicAttributes;

	UPROPERTY()
	UMagicAttributeSet* MagicAttributes;

	TMap<FGameplayTag, FGameplayAbilitySpecHandle> CooldownDataMap;
private:
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnEnergyChanged(const FOnAttributeChangeData& Data);

	void OnAbilityActivation(UGameplayAbility* ActivationAbility);
	void OnAbilityFinished(UGameplayAbility* Ability);
	
	void OnSilencedStatusChanged(FGameplayTag GameplayTag, int32 Count);
	void OnFlashedStatusChanged(FGameplayTag GameplayTag, int32 Count);

	void OnCooldownChanged(FGameplayTag GameplayTag, int32 Count);

};
