#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "GameplayEffectTypes.h"
#include "GASPlayerState.generated.h"

class UBasicAttributeSet;
class UMagicAttributeSet;

UCLASS()
class GASRPG_API AGASPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBasicAttributeSet> BasicAttributeSet;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMagicAttributeSet> MagicAttributeSet;

	virtual void BeginPlay() override;


public:
	AGASPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FORCEINLINE UBasicAttributeSet* GetBasicAttributeSet() const { return BasicAttributeSet; }
	FORCEINLINE UMagicAttributeSet* GetMagicAttributeSet() const { return MagicAttributeSet; }
};
