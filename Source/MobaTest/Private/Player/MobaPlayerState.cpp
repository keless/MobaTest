// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MobaPlayerState.h"
#include "Character/Abilities/AttributeSets/CharacterAttributeSetBase.h"
#include "Character/Abilities/CharacterAbilitySystemComponent.h"

AMobaPlayerState::AMobaPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSetBase = CreateDefaultSubobject<UCharacterAttributeSetBase>(TEXT("AttributeSetBase"));

	NetUpdateFrequency = 100.0f;

	deadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent* AMobaPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCharacterAttributeSetBase* AMobaPlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

bool AMobaPlayerState::isAlive() const
{
	return GetHealth() > 0.0f;
}

void AMobaPlayerState::ShowAbilityConfirmCancelText(bool showText)
{
	// Todo: connect this to HUD when it exists
}

float AMobaPlayerState::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

float AMobaPlayerState::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

float AMobaPlayerState::GetMana() const
{
	return AttributeSetBase->GetMana();
}

float AMobaPlayerState::GetMaxMana() const
{
	return AttributeSetBase->GetMaxMana();
}

int32 AMobaPlayerState::GetCharacterLevel() const
{
	return AttributeSetBase->GetCharacterLevel();
}

void AMobaPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent) {
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &AMobaPlayerState::HealthChanged);
		MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxHealthAttribute()).AddUObject(this, &AMobaPlayerState::MaxHealthChanged);
		ManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetManaAttribute()).AddUObject(this, &AMobaPlayerState::ManaChanged);
		MaxManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxManaAttribute()).AddUObject(this, &AMobaPlayerState::MaxManaChanged);
		CharacterLevelChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetCharacterLevelAttribute()).AddUObject(this, &AMobaPlayerState::CharacterLevelChanged);

		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AMobaPlayerState::StunTagChanged);
	}
}

void AMobaPlayerState::HealthChanged(const FOnAttributeChangeData& data)
{
	UE_LOG(LogTemp, Warning, TEXT("Health Changed"));
}

void AMobaPlayerState::MaxHealthChanged(const FOnAttributeChangeData& data)
{
	UE_LOG(LogTemp, Warning, TEXT("Max Health Changed"));
}

void AMobaPlayerState::ManaChanged(const FOnAttributeChangeData& data)
{
	UE_LOG(LogTemp, Warning, TEXT("ManaChanged"));
}

void AMobaPlayerState::MaxManaChanged(const FOnAttributeChangeData& data)
{
	UE_LOG(LogTemp, Warning, TEXT("MaxManaChanged"));
}

void AMobaPlayerState::CharacterLevelChanged(const FOnAttributeChangeData& data)
{
	UE_LOG(LogTemp, Warning, TEXT("CharacterLevelChanged"));
}

void AMobaPlayerState::StunTagChanged(const FGameplayTag callbackTag, int32 newCount)
{
	UE_LOG(LogTemp, Warning, TEXT("StunTagChanged"));

	if (newCount > 0) {
		FGameplayTagContainer abilityTagsToCancel;
		// On stun, cancel all abilities 
		abilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		FGameplayTagContainer abilityTagsToIgnore;
		// except for any abilities that ignore stun
		abilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCancelledByStun")));

		AbilitySystemComponent->CancelAbilities(&abilityTagsToCancel, &abilityTagsToIgnore);
	}
}
