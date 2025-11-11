#include "Ability.h"

#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "WidgetController.h"
#include "Abilities/GameplayAbility.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "GASRPG/GASRPG.h"
#include "GASRPG/GameplayAbilitySystem/Abilities/BaseGameplayAbility.h"

void UAbility::InitializeFromSpec(const FGameplayAbilitySpec& Spec, UWidgetController* InWidgetController)
{
	//AbilityText->SetText(Spec.Ability.GetClass()->GetDisplayNameText());
	WidgetController = InWidgetController;
	AbilityActivatedImage->SetOpacity(0.f);

	if (!IsValid(WidgetController))
	{
		return;
	}

	if (UGameplayAbility* Ability = Spec.Ability)
	{
		if (UBaseGameplayAbility* BaseAbility = Cast<UBaseGameplayAbility>(Ability))
		{
			AbilityBackground->SetBrushFromTexture(BaseAbility->GetAbilityIcon());
			AbilityKeyBind->SetText(GetInputTextFromInputID(BaseAbility->GetInputID()));
		}
		GameplayAbility = Spec.Ability;
		const FGameplayTagContainer* CooldownTags = Ability->GetCooldownTags();
		if (CooldownTags && !CooldownTags->IsEmpty() && CooldownTags->First().IsValid())
		{
			AbilityCooldownTag = CooldownTags->First();
		}
	}
	WidgetController->OnCooldownStarted.AddDynamic(this, &UAbility::OnCooldownStarted);
	WidgetController->OnCooldownEnded.AddDynamic(this, &UAbility::OnCooldownEnded);

	WidgetController->OnAbilityActivated.AddDynamic(this, &UAbility::OnAbilityActivation);
	WidgetController->OnAbilityEnded.AddDynamic(this, &UAbility::OnAbilityEnded);
}

void UAbility::NativeConstruct()
{
	Super::NativeConstruct();

	CooldownOverlay->SetVisibility(ESlateVisibility::Hidden);
	CooldownDecreaseRate = 0.01f;

	CooldownMID = CooldownBackground->GetDynamicMaterial();
}

void UAbility::OnCooldownStarted(FGameplayTag GameplayTag, const float& Duration, const float& TimeRemaining)
{
	if (GameplayTag == AbilityCooldownTag)
	{
		CooldownTimeRemaining = TimeRemaining;
		CooldownDuration = Duration;

		if (IsValid(CooldownMID))
		{
			const float Progress = CooldownTimeRemaining / CooldownDuration;
			CooldownMID->SetScalarParameterValue("Progress", Progress);
		}

		CooldownOverlay->SetVisibility(ESlateVisibility::Visible);
		CooldownText->SetText(FText::AsNumber(FMath::CeilToInt(CooldownTimeRemaining)));
		
		GetWorld()->GetTimerManager().SetTimer(
			CooldownTimer,
			this,
			&UAbility::UpdateCooldownTimer,
			CooldownDecreaseRate,
			true
		);
	}
}

void UAbility::OnCooldownEnded(FGameplayTag GameplayTag)
{
	if (GameplayTag == AbilityCooldownTag)
	{
		CooldownOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UAbility::OnAbilityActivation(UGameplayAbility* ActivationAbility)
{
	if (!IsValid(ActivationAbility) || !IsValid(GameplayAbility))
	{
		return;
	}
	if (ActivationAbility->GetClass() == GameplayAbility->GetClass())
	{
		AbilityActivatedImage->SetOpacity(0.5f);
	}
}

void UAbility::OnAbilityEnded(UGameplayAbility* Ability)
{
	if (!IsValid(Ability) || !IsValid(GameplayAbility))
	{
		return;
	}

	if (Ability->GetClass() == GameplayAbility->GetClass())
	{
		AbilityActivatedImage->SetOpacity(0.f);
	}
	
}

void UAbility::UpdateCooldownTimer()
{
	CooldownTimeRemaining -= CooldownDecreaseRate;

	if (CooldownTimeRemaining <= KINDA_SMALL_NUMBER)
	{
		CooldownText->SetText(FText::FromString(TEXT("")));
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimer);
		return;
	}

	CooldownText->SetText(FText::AsNumber(FMath::CeilToInt(CooldownTimeRemaining)));
	const float Progress = CooldownTimeRemaining / CooldownDuration;
	CooldownMID->SetScalarParameterValue("Progress", Progress);
}

FText UAbility::GetInputTextFromInputID(EGASAbilityInputID InputID)
{
	switch (InputID)
	{
	case EGASAbilityInputID::None:
		return FText::FromString("");

	case EGASAbilityInputID::Ability1:
		return FText::FromString("Q");

	case EGASAbilityInputID::Ability2:
		return FText::FromString("E");

	case EGASAbilityInputID::Ability3:
		return FText::FromString("C");

	case EGASAbilityInputID::Ability4:
		return FText::FromString("X");
	default:
		return FText::FromString("M");
	}
}

