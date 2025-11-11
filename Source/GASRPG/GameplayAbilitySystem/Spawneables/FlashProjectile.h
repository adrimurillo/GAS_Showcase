#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "GameplayTagContainer.h"
#include "FlashProjectile.generated.h"

class UGameplayEffect;

UCLASS()
class GASRPG_API AFlashProjectile : public ABaseProjectile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flash", meta = (AllowPrivateAccess = true))
	float LifeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flash", meta = (AllowPrivateAccess = true))
	float FlashRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flash", meta = (AllowPrivateAccess = true))
	float FlashDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flash", meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> FlashEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flash", meta = (AllowPrivateAccess = true))
	FGameplayTag FlashDurationTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flash", meta = (AllowPrivateAccess = true))
	float FlashMinDotProduct;
public:
	AFlashProjectile();

	virtual void LaunchProjectile(const FVector& Velocity) override;
	
	void Explode();

protected:
	virtual void BeginPlay() override;

};
