#include "SilenceOverlay.h"

#include "WidgetController.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"

void USilenceOverlay::InitializeWidget(UWidgetController* Controller)
{
	WidgetController = Controller;
	if (IsValid(WidgetController))
	{
		WidgetController->OnSilenceStarted.AddDynamic(this, &USilenceOverlay::OnSilenceStarted);
		WidgetController->OnSilenceEnded.AddDynamic(this, &USilenceOverlay::OnSilenceEnded);
	}
}

void USilenceOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	Overlay->SetVisibility(ESlateVisibility::Hidden);

	SilenceDecreaseRate = 0.1f;
}

void USilenceOverlay::OnSilenceStarted(const float Duration)
{
	if (SilenceTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(SilenceTimer);
	}
	CurrentDuration = Duration;
	SilenceText->SetText(FText::AsNumber(FMath::CeilToInt(CurrentDuration)));
	Overlay->SetVisibility(ESlateVisibility::Visible);

	GetWorld()->GetTimerManager().SetTimer(
		SilenceTimer,
		this,
		&USilenceOverlay::UpdateSilenceTimer,
		SilenceDecreaseRate,
		true
	);
}

void USilenceOverlay::OnSilenceEnded()
{
	Overlay->SetVisibility(ESlateVisibility::Hidden);
}

void USilenceOverlay::UpdateSilenceTimer()
{
	CurrentDuration -= SilenceDecreaseRate;
	if (CurrentDuration <= KINDA_SMALL_NUMBER)
	{
		SilenceText->SetText(FText::FromString(TEXT("")));
		GetWorld()->GetTimerManager().ClearTimer(SilenceTimer);
		return;
	}
	SilenceText->SetText(FText::AsNumber(FMath::CeilToInt(CurrentDuration)));
}
