// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MobaCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Character/Abilities/CharacterAbilitySystemComponent.h"
#include "Character/Abilities/CharacterGameplayAbility.h"
#include "Character/Abilities/AttributeSets/CharacterAttributeSetBase.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AMobaCharacterBase::AMobaCharacterBase(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

	bAlwaysRelevant = true;

	deadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	effectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("State.RemoveOnDeath"));
}

// Called when the game starts or when spawned
void AMobaCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (abilitySystemComponent.IsValid()) {
		HealthChangedDelegateHandle = abilitySystemComponent->GetGameplayAttributeValueChangeDelegate(attributeSetBase->GetHealthAttribute()).AddUObject(this, &AMobaCharacterBase::HealthChanged);
	}
}

void AMobaCharacterBase::AddCharacterAbilities() {
	if (GetLocalRole() != ROLE_Authority || !abilitySystemComponent.IsValid() || abilitySystemComponent->characterAbilitiesGiven) {
		return;
	}

	for (TSubclassOf<UCharacterGameplayAbility>& startupAbility : characterAbilities) {
		auto startupAbilityObject = startupAbility.GetDefaultObject();

		UE_LOG(LogTemp, Log, TEXT("give startup ability"));

		abilitySystemComponent->GiveAbility(FGameplayAbilitySpec(startupAbility, getAbilityLevel(startupAbilityObject->abilityID), static_cast<uint32>(startupAbilityObject->abilityInputID), this));
	}

	abilitySystemComponent->characterAbilitiesGiven = true;
}

void AMobaCharacterBase::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !abilitySystemComponent.IsValid() || !abilitySystemComponent->characterAbilitiesGiven) {
		return;
	}

	TArray<FGameplayAbilitySpecHandle> abilitiesToRemove;
	for (const FGameplayAbilitySpec& spec : abilitySystemComponent->GetActivatableAbilities()) {
		if ((spec.SourceObject == this) && characterAbilities.Contains(spec.Ability->GetClass()))
		{
			abilitiesToRemove.Add(spec.Handle);
		}
	}

	for (int32 i = 0; i < abilitiesToRemove.Num(); i++) {
		abilitySystemComponent->ClearAbility(abilitiesToRemove[i]);
	}

	abilitySystemComponent->characterAbilitiesGiven = false;
}

void AMobaCharacterBase::InitializeAttributes() {
	if (!abilitySystemComponent.IsValid()) {
		return;
	}

	if (!defaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() missing DefaultAttributes for %s. Please fill in the character's blueprint."), *FString(__FUNCTION__), *GetName());
	}

	FGameplayEffectContextHandle effectContext = abilitySystemComponent->MakeEffectContext();
	effectContext.AddSourceObject(this);

	//zzz TODO: add "level" attribute to CharacterAttributeSetBase and use that here
	auto characterLevel = 1;
	FGameplayEffectSpecHandle newHandle = abilitySystemComponent->MakeOutgoingSpec(defaultAttributes, characterLevel, effectContext);
	if (newHandle.IsValid()) {
		FActiveGameplayEffectHandle activeGEHandle = abilitySystemComponent->ApplyGameplayEffectSpecToTarget(*newHandle.Data.Get(), abilitySystemComponent.Get());
	}
}

void AMobaCharacterBase::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !abilitySystemComponent.IsValid() || !abilitySystemComponent->startupEffectsApplied) {
		return;
	}

	FGameplayEffectContextHandle effectContext = abilitySystemComponent->MakeEffectContext();
	effectContext.AddSourceObject(this);

	//zzz TODO: add "level" attribute to CharacterAttributeSetBase and use that here
	auto characterLevel = 1;
	for (TSubclassOf<UGameplayEffect>& startupEffect : startupEffects) {
		UE_LOG(LogTemp, Log, TEXT("try add startup effect"));

		auto newHandle = abilitySystemComponent->MakeOutgoingSpec(startupEffect, characterLevel, effectContext);
		if (newHandle.IsValid()) {

			UE_LOG(LogTemp, Log, TEXT("try apply startup effect"));
			auto activeGEHandle = abilitySystemComponent->ApplyGameplayEffectSpecToTarget(*newHandle.Data.Get(), abilitySystemComponent.Get());
		}
	}

	abilitySystemComponent->startupEffectsApplied = true;
}

void AMobaCharacterBase::SetHealth(float health)
{
	if (attributeSetBase.IsValid()) {
		attributeSetBase->SetHealth(health);

		UE_LOG(LogTemp, Display, TEXT("Set Health Called"));
	}
}

void AMobaCharacterBase::SetMaxHealth(float maxHealth)
{
	if (attributeSetBase.IsValid()) {
		attributeSetBase->SetMaxHealth(maxHealth);
	}
}

void AMobaCharacterBase::SetMana(float mana)
{
	if (attributeSetBase.IsValid()) {
		attributeSetBase->SetMana(mana);
	}
}

void AMobaCharacterBase::SetMaxMana(float maxMana)
{
	if (attributeSetBase.IsValid()) {
		attributeSetBase->SetMaxMana(maxMana);
	}
}

void AMobaCharacterBase::SetCharacterLevel(int32 level)
{
	if (attributeSetBase.IsValid()) {
		attributeSetBase->SetCharacterLevel(level);
	}
}

void AMobaCharacterBase::HealthChanged(const FOnAttributeChangeData& data)
{
	UE_LOG(LogTemp, Log, TEXT("HealthChanged delegate -- broadcast pt2"));
	OnCharacterHealthChangedPt2.Broadcast(this);
}

/*
// Called every frame
void AMobaCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
*/

UAbilitySystemComponent* AMobaCharacterBase::GetAbilitySystemComponent() const
{
	return abilitySystemComponent.Get();
}

bool AMobaCharacterBase::isAlive() const {
	return GetHealth() > 0;
}

int32 AMobaCharacterBase::getAbilityLevel(MobaAbilityID abilityID) const
{
	if (attributeSetBase.IsValid()) {
		return attributeSetBase->GetCharacterLevel();
	}

	return 0;
}

float AMobaCharacterBase::GetHealth() const {
	if (attributeSetBase.IsValid()) {
		return attributeSetBase->GetHealth();
	}

	//todo: throw error?
	return 0.0f;
}

float AMobaCharacterBase::GetMaxHealth() const {
	if (attributeSetBase.IsValid()) {
		return attributeSetBase->GetMaxHealth();
	}

	//todo: throw error?
	return 0.0f;
}

float AMobaCharacterBase::GetMana() const {
	if (attributeSetBase.IsValid()) {
		return attributeSetBase->GetMana();
	}

	//todo: throw error?
	return 0.0f;
}

float AMobaCharacterBase::GetMaxMana() const {
	if (attributeSetBase.IsValid()) {
		return attributeSetBase->GetMaxMana();
	}

	//todo: throw error?
	return 0.0f;
}

void AMobaCharacterBase::die() {
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	OnCharacterDied.Broadcast(this);

	if (abilitySystemComponent.IsValid()) {
		abilitySystemComponent->CancelAbilities();

		FGameplayTagContainer effectsTagsToRemove;
		effectsTagsToRemove.AddTag(effectRemoveOnDeathTag);
		int32 numEffectsRemoved = abilitySystemComponent->RemoveActiveEffectsWithTags(effectsTagsToRemove);
		abilitySystemComponent->AddLooseGameplayTag(deadTag);
	}

	if (deathMontage) {
		PlayAnimMontage(deathMontage);
	}
	else {
		finishDying();
	}
}

void AMobaCharacterBase::finishDying() {
	Destroy();
}

