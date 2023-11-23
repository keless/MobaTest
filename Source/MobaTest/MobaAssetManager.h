// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "MobaAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class MOBATEST_API UMobaAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
	virtual void StartInitialLoading() override;
};
