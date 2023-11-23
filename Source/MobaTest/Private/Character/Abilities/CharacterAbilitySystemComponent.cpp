// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/CharacterAbilitySystemComponent.h"

void UCharacterAbilitySystemComponent::ReceiveDamage(UCharacterAbilitySystemComponent* sourceASC, float unmitigatedDamage, float mitigatedDamage)
{
	receivedDamage.Broadcast(sourceASC, unmitigatedDamage, mitigatedDamage);
}