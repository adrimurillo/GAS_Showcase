#include "FlashOverlay.h"

#include "WidgetController.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "GASRPG/Util/LoggerUtil.h"

void UFlashOverlay::InitializeWidget(UWidgetController* Controller)
{
	WidgetController = Controller;

	if (IsValid(WidgetController))
	{
		WidgetController->OnFlashStarted.AddDynamic(this, &UFlashOverlay::OnFlashStarted);
		WidgetController->OnFlashEnded.AddDynamic(this, &UFlashOverlay::OnFlashEnded);
	}
	
	Overlay->SetVisibility(ESlateVisibility::Hidden);
}

void UFlashOverlay::OnFlashStarted(const float Duration)
{
	Overlay->SetVisibility(ESlateVisibility::Visible);
	TotalEffectDuration = Duration; 
	FadeElapsed = 0.0f;
	BackgroundImage->SetOpacity(1);
	GetWorld()->GetTimerManager().SetTimer(
		FadeTimer, 
		this, 
		&UFlashOverlay::FadeUpdate, 
		FadeUpdateRate, 
		true
	);
}

void UFlashOverlay::OnFlashEnded()
{
	Overlay->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(FadeTimer);
}

void UFlashOverlay::FadeUpdate()
{
	FadeElapsed += FadeUpdateRate;
	if (TotalEffectDuration - FadeElapsed <= FadeDuration)
	{
		float Alpha = (TotalEffectDuration - FadeElapsed) / FadeDuration;
		Alpha = FMath::Clamp(Alpha, 0.f, 1.f);

		BackgroundImage->SetOpacity(Alpha);
	}
}
