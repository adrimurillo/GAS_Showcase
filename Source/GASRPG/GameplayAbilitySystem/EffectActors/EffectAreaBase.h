// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "EffectAreaBase.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FEffectProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
	FGameplayTag DataTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
	float Magnitude;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
	float EffectDurationTimeAfterEndOverlap;
};

UCLASS()
class GASRPG_API AEffectAreaBase : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereComp;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	TArray<FEffectProperties> EffectProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	float AreaRadius;
	
public:
	AEffectAreaBase();
	void SetAreaRadius(const float Radius);

protected:
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
};
