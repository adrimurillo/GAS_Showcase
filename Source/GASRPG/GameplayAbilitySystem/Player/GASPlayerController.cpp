#include "GASPlayerController.h"

#include "AbilitySystemComponent.h"
#include "GASPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "GASRPG/GameplayAbilitySystem/UI/MainHud.h"
#include "GASRPG/GameplayAbilitySystem/UI/WidgetController.h"
#include "GASRPG/Util/LoggerUtil.h"

void AGASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		CreateHUD();
	}
}

void AGASPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (IsLocalController())
	{
		CreateHUD();
	}
}

void AGASPlayerController::CreateHUD()
{
	if (HUD || !IsValid(HUDClass))
	{
		return;
	}

	AGASPlayerState* PS = GetPlayerState<AGASPlayerState>();
	if (!IsValid(PS))
	{
		return;
	}

	HUD = CreateWidget<UMainHud>(this, HUDClass);
	HUD->AddToViewport();
	UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
	
	WidgetController = NewObject<UWidgetController>(this);
	WidgetController->Initialize(ASC, PS->GetBasicAttributeSet(), PS->GetMagicAttributeSet());

	HUD->Initialize(WidgetController, ASC->GetActivatableAbilities());
	
}
