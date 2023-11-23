// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Character/Abilities/AttributeSets/CharacterAttributeSetBase.h"
#include "MobaPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MOBATEST_API AMobaPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AMobaPlayerState();

	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UCharacterAttributeSetBase* GetAttributeSetBase() const;

	UFUNCTION(BlueprintCallable, Category = "Moba|PlayerState")
	bool isAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Moba|PlayerState|UI")
	void ShowAbilityConfirmCancelText(bool showText);

	UFUNCTION(BlueprintCallable, Category = "Moba|PlayerState|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Moba|PlayerState|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Moba|PlayerState|Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "Moba|PlayerState|Attributes")
	float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "Moba|PlayerState|Attributes")
	int32 GetCharacterLevel() const;

protected:
	UPROPERTY()
	class UCharacterAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class UCharacterAttributeSetBase* AttributeSetBase;

	FGameplayTag deadTag;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle ManaChangedDelegateHandle;
	FDelegateHandle MaxManaChangedDelegateHandle;
	FDelegateHandle CharacterLevelChangedDelegateHandle;

	virtual void BeginPlay() override;

	virtual void HealthChanged(const FOnAttributeChangeData& data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& data);
	virtual void ManaChanged(const FOnAttributeChangeData& data);
	virtual void MaxManaChanged(const FOnAttributeChangeData& data);
	virtual void CharacterLevelChanged(const FOnAttributeChangeData& data);

	virtual void StunTagChanged(const FGameplayTag callbackTag, int32 newCount);
};
