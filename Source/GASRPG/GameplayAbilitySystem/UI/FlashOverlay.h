#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FlashOverlay.generated.h"

class UWidgetController;
class UOverlay;
class UImage;

UCLASS()
class GASRPG_API UFlashOverlay : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget), VisibleAnywhere)
	UOverlay* Overlay;

	UPROPERTY(meta = (BindWidget), VisibleAnywhere)
	UImage* BackgroundImage;

	UPROPERTY()
	UWidgetController* WidgetController;

	FTimerHandle FadeTimer;
	float TotalEffectDuration;
	float FadeDuration = 1.f;
	float FadeElapsed = 0.0f;
	float FadeUpdateRate = 0.01f;

public:
	void InitializeWidget(UWidgetController* Controller);

private:
	UFUNCTION()
	void OnFlashStarted(const float Duration);

	UFUNCTION()
	void OnFlashEnded();

	void FadeUpdate();
};
