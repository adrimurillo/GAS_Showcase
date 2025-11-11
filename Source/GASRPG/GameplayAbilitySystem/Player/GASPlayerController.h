#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GASPlayerController.generated.h"

class UWidgetController;
class UMainHud;

UCLASS()
class GASRPG_API AGASPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;
	virtual void OnRep_PlayerState() override;

private:
	UPROPERTY()
	UMainHud* HUD;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMainHud> HUDClass;

	UPROPERTY()
	UWidgetController* WidgetController;
	
	void CreateHUD();
};
