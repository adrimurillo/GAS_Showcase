#pragma once

#include "CoreMinimal.h"
#include "Flash.h"
#include "NormalFlash.generated.h"

UCLASS()
class GASRPG_API UNormalFlash : public UFlash
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Flash", meta = (AllowPrivateAccess = true))
	float ThrowForce;

protected:
	
	UFUNCTION()
	virtual void OnTargetDataReady(const FGameplayAbilityTargetDataHandle& Data) override;

	UFUNCTION()
	virtual void OnTargetCancelled(const FGameplayAbilityTargetDataHandle& Data) override;
};
