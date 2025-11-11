#include "WidgetController.h"

#include "AbilitySystemComponent.h"
#include "GASRPG/GameplayAbilitySystem/Attributes/BasicAttributeSet.h"
#include "GASRPG/Util/LoggerUtil.h"

void UWidgetController::Initialize(UAbilitySystemComponent* InASC, UBasicAttributeSet* InBasicAttributes,
                                   UMagicAttributeSet* InMagicAttributes)
{
	ASC = InASC;
	BasicAttributes = InBasicAttributes;
	MagicAttributes = InMagicAttributes;

	if (!ASC)
	{
		return;
	}

	ASC->GetGameplayAttributeValueChangeDelegate(BasicAttributes->GetHealthAttribute()).AddUObject(this, &UWidgetController::OnHealthChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(BasicAttributes->GetEnergyAttribute()).AddUObject(this, &UWidgetController::OnEnergyChanged);

	ASC->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("Status.Silenced"), EGameplayTagEventType::AnyCountChange).AddUObject(this, &UWidgetController::OnSilencedStatusChanged);
	ASC->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("Status.Flashed"), EGameplayTagEventType::AnyCountChange).AddUObject(this, &UWidgetController::OnFlashedStatusChanged);

	ASC->AbilityActivatedCallbacks.AddUObject(this, &UWidgetController::OnAbilityActivation);
	ASC->AbilityEndedCallbacks.AddUObject(this, &UWidgetController::OnAbilityFinished);
	// Cooldowns
	for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		if (!IsValid(Spec.Ability) || !IsValid(Spec.Ability->GetCooldownGameplayEffect()))
		{
			continue;
		}
		const FGameplayTag CooldownTag =
			Spec.Ability->GetCooldownGameplayEffect()->GetGrantedTags().Num() > 0
				? Spec.Ability->GetCooldownGameplayEffect()->GetGrantedTags().First()
				: FGameplayTag();

		if (CooldownTag.IsValid())
		{
			ASC->RegisterGameplayTagEvent(CooldownTag).AddUObject(this, &UWidgetController::OnCooldownChanged);
			CooldownDataMap.Add(CooldownTag, Spec.Handle);
		}
	}
}

void UWidgetController::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	OnHealthUpdated.Broadcast(Data.NewValue, BasicAttributes->GetMaxHealth());
}

void UWidgetController::OnEnergyChanged(const FOnAttributeChangeData& Data)
{
	OnEnergyUpdated.Broadcast(Data.NewValue, BasicAttributes->GetMaxEnergy());
}

void UWidgetController::OnAbilityActivation(UGameplayAbility* ActivationAbility)
{
	OnAbilityActivated.Broadcast(ActivationAbility);
}

void UWidgetController::OnAbilityFinished(UGameplayAbility* Ability)
{
	OnAbilityEnded.Broadcast(Ability);
}

void UWidgetController::OnSilencedStatusChanged(FGameplayTag GameplayTag, int32 Count)
{	
	if (Count > 0)
	{
		float EndTime = 0.f;
		float TotalDuration = 0.f;
		FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(
			FGameplayTagContainer(GameplayTag)
		);
		bool bFound = ASC->GetActiveEffectsEndTimeAndDuration(
			Query,
			EndTime,
			TotalDuration
		);

		OnSilenceStarted.Broadcast(TotalDuration);
	}
	else
	{
		OnSilenceEnded.Broadcast();
	}
}

void UWidgetController::OnFlashedStatusChanged(FGameplayTag GameplayTag, int32 Count)
{
	if (Count > 0)
	{
		float EndTime = 0.f;
		float TotalDuration = 0.f;
		FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(
			FGameplayTagContainer(GameplayTag)
		);
		bool bFound = ASC->GetActiveEffectsEndTimeAndDuration(
			Query,
			EndTime,
			TotalDuration
		);
		OnFlashStarted.Broadcast(TotalDuration);
	}
	else
	{
		OnFlashEnded.Broadcast();
	}
}

void UWidgetController::OnCooldownChanged(FGameplayTag GameplayTag, int32 Count)
{
	if (Count > 0)
	{
		float Duration;
		float TimeRemaining;
		FGameplayAbilityActorInfo* ActorInfo = ASC->AbilityActorInfo.Get();
		FGameplayAbilitySpecHandle* CdSpecHandle = CooldownDataMap.Find(GameplayTag);

		FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(*CdSpecHandle);
		if (!Spec || !Spec->Ability)
		{
			return;
		}
		Spec->Ability->GetCooldownTimeRemainingAndDuration(*CdSpecHandle, ActorInfo, Duration, TimeRemaining);
		OnCooldownStarted.Broadcast(GameplayTag, Duration, TimeRemaining);
	}
	else
	{
		OnCooldownEnded.Broadcast(GameplayTag);
	}
}

