#include "MainHud.h"

#include "Ability.h"
#include "FlashOverlay.h"
#include "GameplayAbilitySpec.h"
#include "SilenceOverlay.h"
#include "WidgetController.h"
#include "Abilities/GameplayAbility.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/ProgressBar.h"

void UMainHud::Initialize(UWidgetController* Controller, const TArray<FGameplayAbilitySpec>& Specs)
{
	SetWidgetController(Controller);
	BindDelegates();
	SetWidgetAbilities(Specs);
	SilenceOverlay->InitializeWidget(Controller);
	FlashOverlay->InitializeWidget(Controller);
}

void UMainHud::BindDelegates()
{
	if (!WidgetController)
	{
		return;
	}

	WidgetController->OnHealthUpdated.AddDynamic(this, &UMainHud::OnHealthChanged);
	WidgetController->OnEnergyUpdated.AddDynamic(this, &UMainHud::OnEnergyChanged);
	
}

void UMainHud::SetWidgetAbilities(const TArray<FGameplayAbilitySpec>& Specs)
{
	for (const FGameplayAbilitySpec& Spec : Specs)
	{
		UAbility* AbilityWidget = CreateWidget<UAbility>(GetWorld(), AbilityWidgetClass);
		if (!IsValid(AbilityWidget))
		{
			continue;
		}

		FGameplayTagContainer AssetTags = Spec.Ability->GetAssetTags();
		if (AssetTags.Num() > 0 && AssetTags.HasTag(FGameplayTag::RequestGameplayTag("GameplayAbility.Passive")))
		{
			continue;
		}

		AbilityWidget->InitializeFromSpec(Spec, WidgetController);
		UHorizontalBoxSlot* AbilitySlot = Cast<UHorizontalBoxSlot>(AbilitiesHorizontalBox->AddChild(AbilityWidget));
		if (IsValid(AbilitySlot))
		{
			AbilitySlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
			AbilitySlot->SetVerticalAlignment(VAlign_Center);
			AbilitySlot->SetHorizontalAlignment(HAlign_Center);
		}
	}
}

void UMainHud::NativeConstruct()
{
	Super::NativeConstruct();
	HealthProgressBar->SetPercent(1);
	EnergyProgressBar->SetPercent(1);
}

void UMainHud::OnHealthChanged(float NewValue, float MaxValue)
{
	HealthProgressBar->SetPercent(NewValue / MaxValue);
}

void UMainHud::OnEnergyChanged(float NewValue, float MaxValue)
{
	EnergyProgressBar->SetPercent(NewValue / MaxValue);
}

