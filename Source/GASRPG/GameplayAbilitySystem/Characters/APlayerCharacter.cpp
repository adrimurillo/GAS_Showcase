#include "APlayerCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GASRPG/GameplayAbilitySystem/Attributes/BasicAttributeSet.h"
#include "GASRPG/GameplayAbilitySystem/Weapons/WeaponManager.h"
#include "GASRPG/Util/LoggerUtil.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GASRPG/GASRPG.h"
#include "GASRPG/GameplayAbilitySystem/Abilities/Dash.h"
#include "GASRPG/GameplayAbilitySystem/Attributes/MagicAttributeSet.h"
#include "GASRPG/GameplayAbilitySystem/Player/GASPlayerState.h"


AAPlayerCharacter::AAPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	WeaponComponent = CreateDefaultSubobject<UWeaponManager>(TEXT("Weapon Component"));
	WeaponComponent->SetIsReplicated(true);

	ASCInputBound = false;
}

void AAPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Server Only
void AAPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AGASPlayerState* PS = GetPlayerState<AGASPlayerState>();
	if (IsValid(PS))
	{
		AbilitySystemComponent = PS->GetAbilitySystemComponent();
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		BasicAttributeSet = PS->GetBasicAttributeSet();
		MagicAttributeSet = PS->GetMagicAttributeSet();

		AddCharacterAbilities();
		
		AbilitySystemComponent
			->GetGameplayAttributeValueChangeDelegate(BasicAttributeSet->GetEnergyAttribute())
			.AddUObject(this, &AAPlayerCharacter::OnEnergyChanged);
		AbilitySystemComponent
			->GetGameplayAttributeValueChangeDelegate(MagicAttributeSet->GetManaAttribute())
			.AddUObject(this, &AAPlayerCharacter::OnManaChanged);
		AbilitySystemComponent
			->GetGameplayAttributeValueChangeDelegate(BasicAttributeSet->GetMovementSpeedAttribute())
			.AddUObject(this, &AAPlayerCharacter::OnMovementSpeedChanged);
	}
}

// Client Only
void AAPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AGASPlayerState* PS = GetPlayerState<AGASPlayerState>();
	if (IsValid(PS))
	{
		AbilitySystemComponent = PS->GetAbilitySystemComponent();
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		BasicAttributeSet = PS->GetBasicAttributeSet();
		MagicAttributeSet = PS->GetMagicAttributeSet();

		BindASCInput();

		//TArray<FGameplayAttribute> GameplayAttributes;
		//GameplayAttributes.Add(BasicAttributeSet->GetEnergyAttribute());
		//GameplayAttributes.Add(MagicAttributeSet->GetManaAttribute());
		//UAsyncTaskAttributeChanged* Task = UAsyncTaskAttributeChanged::ListenForAttributesChange(AbilitySystemComponent, GameplayAttributes);
		//Task->OnAttributeChanged.AddDynamic(this, &AAPlayerCharacter::OnRegenerableAttributeChanged);
		AbilitySystemComponent
			->GetGameplayAttributeValueChangeDelegate(BasicAttributeSet->GetMovementSpeedAttribute())
			.AddUObject(this, &AAPlayerCharacter::OnMovementSpeedChanged);
	}
}


void AAPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!HasAuthority() || !IsValid(AbilitySystemComponent))
		return;
	// TODO remove magic numbers
	const float Speed = GetVelocity().Size();
	const bool bIsDetained = AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Movement.Detained")));

	if (Speed < 5.f && !bIsDetained)
	{
		AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Movement.Detained")));
	}
	else if (Speed >= 5.f && bIsDetained)
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Movement.Detained")));
	}
}

void AAPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);

	if (!IsValid(EnhancedInput))
	{
		FLogger::ErrorLog("Invalid UEnhancedInputComponent pointer in PlayerCharacter.SetupPlayerInputComponent()");
		return;
	}

	BindASCInput();
	
	EnhancedInput->BindAction(Ability1InputAction, ETriggerEvent::Started, this, &AAPlayerCharacter::Ability1InputTriggered);
	EnhancedInput->BindAction(Ability2InputAction, ETriggerEvent::Started, this, &AAPlayerCharacter::Ability2InputTriggered);
	EnhancedInput->BindAction(Ability3InputAction, ETriggerEvent::Started, this, &AAPlayerCharacter::Ability3InputTriggered);
	EnhancedInput->BindAction(Ability4InputAction, ETriggerEvent::Started, this, &AAPlayerCharacter::Ability4InputTriggered);
	EnhancedInput->BindAction(Weapon1InputAction, ETriggerEvent::Started, this, &AAPlayerCharacter::Weapon1InputTriggered);
	EnhancedInput->BindAction(Weapon2InputAction, ETriggerEvent::Started, this, &AAPlayerCharacter::Weapon2InputTriggered);
	EnhancedInput->BindAction(Weapon3InputAction, ETriggerEvent::Started, this, &AAPlayerCharacter::Weapon3InputTriggered);
	EnhancedInput->BindAction(ConfirmAction, ETriggerEvent::Triggered, this, &AAPlayerCharacter::OnConfirmPressed);
	EnhancedInput->BindAction(CancelAction, ETriggerEvent::Triggered, this, &AAPlayerCharacter::OnCancelPressed);
}

void AAPlayerCharacter::Ability1InputTriggered(const FInputActionValue& Value)
{
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}
	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(EGASAbilityInputID::Ability1));
}

void AAPlayerCharacter::Ability2InputTriggered(const FInputActionValue& Value)
{
	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(EGASAbilityInputID::Ability2));
}

void AAPlayerCharacter::Ability3InputTriggered(const FInputActionValue& Value)
{
	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(EGASAbilityInputID::Ability3));
}

void AAPlayerCharacter::Ability4InputTriggered(const FInputActionValue& Value)
{
	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(EGASAbilityInputID::Ability4));
}

void AAPlayerCharacter::Weapon1InputTriggered(const FInputActionValue& Value)
{
	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(EGASAbilityInputID::Weapon1));
}

void AAPlayerCharacter::Weapon2InputTriggered(const FInputActionValue& Value)
{
	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(EGASAbilityInputID::Weapon2));
}

void AAPlayerCharacter::Weapon3InputTriggered(const FInputActionValue& Value)
{
	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(EGASAbilityInputID::Weapon3));
}

void AAPlayerCharacter::OnConfirmPressed(const FInputActionValue& Value)
{
	AbilitySystemComponent->LocalInputConfirm();
}

void AAPlayerCharacter::OnCancelPressed(const FInputActionValue& Value)
{
	AbilitySystemComponent->LocalInputCancel();
}


void AAPlayerCharacter::HandleAttributeRegen(const FGameplayTag GameplayTag, const TSubclassOf<UGameplayEffect> EffectClass, const float NewValue, const float OldValue) const
{
	if (NewValue < OldValue)
	{
		FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		UGameplayEffect* GameplayEffect = EffectClass->GetDefaultObject<UGameplayEffect>();

		AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(GameplayTag));
		AbilitySystemComponent->ApplyGameplayEffectToSelf(GameplayEffect, 0, EffectContextHandle);
	}
	else if (NewValue >= BasicAttributeSet->GetMaxEnergy())
	{
		AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(GameplayTag));
	}
}

void AAPlayerCharacter::OnEnergyChanged(const FOnAttributeChangeData& AttributeChangeData)
{
	HandleAttributeRegen(FGameplayTag::RequestGameplayTag(FName("Status.Energy.Regen")), EnergyRegenEffectClass, AttributeChangeData.NewValue, AttributeChangeData.OldValue);
}

void AAPlayerCharacter::OnManaChanged(const FOnAttributeChangeData& AttributeChangeData)
{
	HandleAttributeRegen(FGameplayTag::RequestGameplayTag(FName("Status.Mana.Regen")), ManaRegenEffectClass, AttributeChangeData.NewValue, AttributeChangeData.OldValue);
}

void AAPlayerCharacter::OnMovementSpeedChanged(const FOnAttributeChangeData& AttributeChangeData)
{
	GetCharacterMovement()->MaxWalkSpeed = AttributeChangeData.NewValue;
	if (!IsValid(Controller))
	{
		return;
	}
	if (AttributeChangeData.NewValue <= 5.f)
	{
		GetCharacterMovement()->StopMovementImmediately();
		//GetCharacterMovement()->DisableMovement();
		Controller->SetIgnoreMoveInput(true);
	}
	else
	{
		Controller->SetIgnoreMoveInput(false);
	}
}

void AAPlayerCharacter::BindASCInput()
{
	if (!ASCInputBound && IsValid(AbilitySystemComponent) && IsValid(InputComponent))
	{
		const FTopLevelAssetPath EnumAssetPath(*StaticEnum<EGASAbilityInputID>()->GetPathName());

		AbilitySystemComponent->BindAbilityActivationToInputComponent(
			InputComponent,
			FGameplayAbilityInputBinds(
				TEXT("ConfirmTarget"),
				TEXT("CancelTarget"),
				EnumAssetPath,
				static_cast<int32>(EGASAbilityInputID::Confirm),
				static_cast<int32>(EGASAbilityInputID::Cancel)
			)
		);

		ASCInputBound = true;
	}
}

