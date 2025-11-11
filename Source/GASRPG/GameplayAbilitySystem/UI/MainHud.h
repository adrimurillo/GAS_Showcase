#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHud.generated.h"

class UFlashOverlay;
class USilenceOverlay;
struct FGameplayAbilitySpec;
class UAbility;
class UHorizontalBox;
class UProgressBar;
class UVerticalBox;
class UWidgetController;

UCLASS()
class GASRPG_API UMainHud : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = true), BlueprintReadOnly)
	UVerticalBox* VerticalBox;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = true), BlueprintReadOnly)
	UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = true), BlueprintReadOnly)
	UProgressBar* EnergyProgressBar;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = true), BlueprintReadOnly)
	UHorizontalBox* AbilitiesHorizontalBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn=true, AllowPrivateAccess=true))
	TSubclassOf<UAbility> AbilityWidgetClass;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = true), BlueprintReadOnly)
	USilenceOverlay* SilenceOverlay;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = true), BlueprintReadOnly)
	UFlashOverlay* FlashOverlay;

public:
	FORCEINLINE void SetWidgetController(UWidgetController* Controller){ WidgetController = Controller; }

	void Initialize(UWidgetController* Controller, const TArray<FGameplayAbilitySpec>& Specs);

	void BindDelegates();

	void SetWidgetAbilities(const TArray<FGameplayAbilitySpec>& Specs);

private:

	UPROPERTY()
	UWidgetController* WidgetController;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnHealthChanged(float NewValue, float MaxValue);

	UFUNCTION()
	void OnEnergyChanged(float NewValue, float MaxValue);
};

