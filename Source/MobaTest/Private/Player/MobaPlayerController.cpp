// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MobaPlayerController.h"
#include "Player/MobaPlayerState.h"
#include "AbilitySystemComponent.h"

void AMobaPlayerController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	AMobaPlayerState* playerState = GetPlayerState<AMobaPlayerState>();
	if (playerState != NULL) {
		playerState->GetAbilitySystemComponent()->InitAbilityActorInfo(playerState, pawn);
	}
}

// TODO: add hud updating code