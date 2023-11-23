// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MobaCharacterBase.h"
#include "Player/MobaPlayerState.h"

#include <InputActionValue.h>

#include "MobaPlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class MOBATEST_API AMobaPlayerCharacter : public AMobaCharacterBase
{
	GENERATED_BODY()

public:
	AMobaPlayerCharacter(const class FObjectInitializer& objectInitializer);

	virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;

	virtual void PossessedBy(AController* newController) override;

	class USpringArmComponent* GetCameraBoom();

	class UCameraComponent* GetFollowCamera();

	UFUNCTION(BlueprintCallable, Category = "Moba|Camera")
	float GetStartingCameraBoomArmLength();

	UFUNCTION(BlueprintCallable, Category = "Moba|Camera")
	FVector GetStartingCameraBoomLocation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|EnhancedInput")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* Action1;

protected:

	void InitializeStartingValues(AMobaPlayerState* playerState);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Moba|Camera")
	float BaseTurnRate = 45.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Moba|Camera")
	float BaseLookUpRate = 45.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Moba|Camera")
	float StartingCameraBoomArmLength;

	UPROPERTY(BlueprintReadOnly, Category = "Moba|Camera")
	FVector StartingCameraBoomLocation;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Moba|Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Moba|Camera")
	class UCameraComponent* FollowCamera;

	bool ASCInputBound = false;

	FGameplayTag DeadTag;

	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState() override;

	void BindASCInput();

	void fnAction1(const FInputActionValue& Value);
};
