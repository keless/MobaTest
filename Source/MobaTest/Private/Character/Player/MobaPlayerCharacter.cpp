// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/MobaPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "AI/PlayerAIController.h"
#include "Player/MobaPlayerController.h"
#include "Character/Abilities/CharacterAbilitySystemComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "EnhancedInputComponent.h"
#include <EnhancedInputSubsystems.h>
#include <InputActionValue.h>

AMobaPlayerCharacter::AMobaPlayerCharacter(const FObjectInitializer& objectInitializer) : Super(objectInitializer)
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocation(FVector(0, 0, 70));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(FName("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->FieldOfView = 80.0f;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	AIControllerClass = APlayerAIController::StaticClass();

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

void AMobaPlayerCharacter::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);

	/* old input system
	//todo; this is old input system shit-- need to hook up enhance input system stuff
	playerInputComponent->BindAxis("MoveForward", this, &AMobaPlayerCharacter::MoveForward);
	playerInputComponent->BindAxis("MoveRight", this, &AMobaPlayerCharacter::MoveRight);
	playerInputComponent->BindAxis("LookUp", this, &AMobaPlayerCharacter::LookUp);
	playerInputComponent->BindAxis("LookUpRate", this, &AMobaPlayerCharacter::LookUpRate);
	playerInputComponent->BindAxis("Turn", this, &AMobaPlayerCharacter::Turn);
	playerInputComponent->BindAxis("TurnRate", this, &AMobaPlayerCharacter::TurnRate);

	BindASCInput();
	*/

	APlayerController* playerController = Cast<APlayerController>(GetController());

	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());

	subsystem->ClearAllMappings();
	subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(playerInputComponent);
	input->BindAction(Action1, ETriggerEvent::Triggered, this, &AMobaPlayerCharacter::fnAction1);
}

void AMobaPlayerCharacter::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);

	AMobaPlayerState* playerState = GetPlayerState<AMobaPlayerState>();
	if (playerState) {
		InitializeStartingValues(playerState);

		AddStartupEffects();
		AddCharacterAbilities();
	}
}

USpringArmComponent* AMobaPlayerCharacter::GetCameraBoom()
{
	return CameraBoom;
}

UCameraComponent* AMobaPlayerCharacter::GetFollowCamera()
{
	return FollowCamera;
}

float AMobaPlayerCharacter::GetStartingCameraBoomArmLength()
{
	return StartingCameraBoomArmLength;
}

FVector AMobaPlayerCharacter::GetStartingCameraBoomLocation()
{
	return StartingCameraBoomLocation;
}

void AMobaPlayerCharacter::InitializeStartingValues(AMobaPlayerState* playerState)
{
	if (playerState) {
		abilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(playerState->GetAbilitySystemComponent());

		playerState->GetAbilitySystemComponent()->InitAbilityActorInfo(playerState, this);

		attributeSetBase = playerState->GetAttributeSetBase();

		abilitySystemComponent->SetTagMapCount(DeadTag, 0);

		InitializeAttributes();
		SetHealth(GetMaxHealth());
		SetMana(GetMaxMana());
	}
}

void AMobaPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMobaPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AMobaPlayerState* playerState = GetPlayerState<AMobaPlayerState>();
	if (playerState) {
		InitializeStartingValues(playerState);

		//BindASCInput();
	}
}

void AMobaPlayerCharacter::BindASCInput()
{
	if (!ASCInputBound && abilitySystemComponent.IsValid() && IsValid(InputComponent)) {
		abilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, 
			FGameplayAbilityInputBinds(FString("ConfirmTarget"), FString("CancelTarget"), 
				FTopLevelAssetPath(GetPathNameSafe(UClass::TryFindTypeSlow<UEnum>("EDGAbilityInputID"))),
			static_cast<int32>(MobaAbilityID::Confirm), static_cast<int32>(MobaAbilityID::Cancel)));

		ASCInputBound = true;
	}
}


void AMobaPlayerCharacter::fnAction1(const FInputActionValue& Value)
{
	//bool BoolValue = Value.Get<bool>(); // for digital input actions
	//float FloatValue = Value.Get<float>(); // for Axis1D input actions
	//FVector2D Axis2DValue = Value.Get<FVector2D>(); // for Axis2D input actions
	//FVector VectorValue = Value.Get<FVector>(); // for Axis3D input actions

	//TODO: fire GameAbility

	UE_LOG(LogTemp, Warning, TEXT("Action1 triggered!"));
}