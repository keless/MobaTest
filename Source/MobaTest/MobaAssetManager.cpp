// Fill out your copyright notice in the Description page of Project Settings.


#include "MobaAssetManager.h"
#include "AbilitySystemGlobals.h"

void UMobaAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
	UE_LOG(LogTemp, Warning, TEXT("Moba Asset Manager init"));
}