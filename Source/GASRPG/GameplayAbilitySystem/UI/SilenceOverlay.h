#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SilenceOverlay.generated.h"

class UWidgetController;
class UOverlay;
class UTextBlock;
class UImage;

UCLASS()
class GASRPG_API USilenceOverlay : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget), VisibleAnywhere)
	UOverlay* Overlay;

	UPROPERTY(meta = (BindWidget), VisibleAnywhere)
	UImage* BackgroundImage;

	UPROPERTY(meta = (BindWidget), VisibleAnywhere)
	UTextBlock* SilenceText;

	UPROPERTY()
	UWidgetController* WidgetController;

	FTimerHandle SilenceTimer;
	float SilenceDecreaseRate;
	float CurrentDuration;

public:
	void InitializeWidget(UWidgetController* Controller);

private:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnSilenceStarted(const float Duration);

	UFUNCTION()
	void OnSilenceEnded();

	void UpdateSilenceTimer();
};
