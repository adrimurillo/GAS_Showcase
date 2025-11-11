// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "BasicAttributeSet.generated.h"


UCLASS()
class GASRPG_API UBasicAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UBasicAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|MovementSpeed", ReplicatedUsing=OnRep_MovementSpeed)
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS_BASIC(UBasicAttributeSet, MovementSpeed);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Health", ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(UBasicAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Health", ReplicatedUsing=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(UBasicAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Health", ReplicatedUsing=OnRep_HealthRegenRate)
	FGameplayAttributeData HealthRegenRate;
	ATTRIBUTE_ACCESSORS_BASIC(UBasicAttributeSet, HealthRegenRate);

	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Energy", ReplicatedUsing=OnRep_MaxEnergy)
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS_BASIC(UBasicAttributeSet, Energy);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Energy", ReplicatedUsing=OnRep_MaxEnergy)
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS_BASIC(UBasicAttributeSet, MaxEnergy);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Energy", ReplicatedUsing=OnRep_EnergyRegenRate)
	FGameplayAttributeData EnergyRegenRate;
	ATTRIBUTE_ACCESSORS_BASIC(UBasicAttributeSet, EnergyRegenRate);


	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Gold", ReplicatedUsing=OnRep_Gold)
	FGameplayAttributeData Gold;
	ATTRIBUTE_ACCESSORS_BASIC(UBasicAttributeSet, Gold);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Experience", ReplicatedUsing=OnRep_XP)
	FGameplayAttributeData XP;
	ATTRIBUTE_ACCESSORS_BASIC(UBasicAttributeSet, XP);
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UFUNCTION()
	virtual void OnRep_MovementSpeed(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_HealthRegenRate(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Energy(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxEnergy(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_EnergyRegenRate(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Gold(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_XP(const FGameplayAttributeData& OldValue);
};
