// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CharacterAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UCharacterAbilitySystemComponent*, sourceASC, float, unmitigatedDamage, float, mitigatedDamage);

/**
 * 
 */
UCLASS()
class MOBATEST_API UCharacterAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	bool characterAbilitiesGiven = false;
	bool startupEffectsApplied = false;

	FReceivedDamageDelegate receivedDamage;

	virtual void ReceiveDamage(UCharacterAbilitySystemComponent* sourceASC, float unmitigatedDamage, float mitigatedDamage);

	//zzz todo: GiveCharacterAbility
};
