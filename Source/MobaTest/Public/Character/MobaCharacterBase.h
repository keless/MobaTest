// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h" //zzz todo: prob dont need this in .h?
#include "GameplayTagContainer.h"

#include <MobaTest/MobaTest.h>


// Generated headers must be last
#include "MobaCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, AMobaCharacterBase*, character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterHealthChangedPt2Delegate, AMobaCharacterBase*, character);


UCLASS()
class MOBATEST_API AMobaCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	AMobaCharacterBase(const class FObjectInitializer& objectInitializer);

	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// implementation IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(BlueprintAssignable, Category = "Moba|Character")
	FCharacterDiedDelegate OnCharacterDied;

	UPROPERTY(BlueprintAssignable, Category = "Moba|Character|Health")
	FCharacterHealthChangedPt2Delegate OnCharacterHealthChangedPt2;

	UFUNCTION(BlueprintCallable, Category = "Moba|Character")
	virtual bool isAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Moba|Character")
	virtual int32 getAbilityLevel(MobaAbilityID abilityID) const;

	/// <summary>
	///  methods called by server only
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Moba|Character")
	virtual void RemoveCharacterAbilities();
	UFUNCTION(BlueprintCallable, Category = "Moba|Character")
	virtual void die();
	UFUNCTION(BlueprintCallable, Category = "Moba|Character")
	virtual void finishDying();

	UFUNCTION(BlueprintCallable, Category = "Moba|Character|Attributes")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Moba|Character|Attributes")
	float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Moba|Character|Attributes")
	float GetMana() const;
	UFUNCTION(BlueprintCallable, Category = "Moba|Character|Attributes")
	float GetMaxMana() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TWeakObjectPtr<class UCharacterAbilitySystemComponent> abilitySystemComponent;
	TWeakObjectPtr<class UCharacterAttributeSetBase> attributeSetBase;

	// Indicates that the character is dead
	FGameplayTag deadTag;

	// Indicates the character finished dying, and can be removed from game
	FGameplayTag effectRemoveOnDeathTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Moba|Character")
	FText characterName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Moba|Animation")
	UAnimMontage* deathMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Moba|Abilities")
	TArray<TSubclassOf<class UCharacterGameplayAbility>> characterAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Moba|Abilities")
	TSubclassOf<class UGameplayEffect> defaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Moba|Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> startupEffects;

	virtual void AddCharacterAbilities();
	virtual void InitializeAttributes();
	virtual void AddStartupEffects();

	virtual void SetHealth(float health);
	virtual void SetMaxHealth(float maxHealth);
	virtual void SetMana(float mana);
	virtual void SetMaxMana(float maxMana);
	virtual void SetCharacterLevel(int32 level);

	
	FDelegateHandle HealthChangedDelegateHandle;
	virtual void HealthChanged(const FOnAttributeChangeData& data);

};
