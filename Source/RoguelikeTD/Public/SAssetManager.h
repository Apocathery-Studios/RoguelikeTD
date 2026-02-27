// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "SAssetManager.generated.h"

class USAbilityDatabase;
/**
 * 
 */
UCLASS()
class ROGUELIKETD_API USAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static USAssetManager& Get();

	UFUNCTION(BlueprintPure, Category = "AssetManager", DisplayName = "Get Ability Database")
	static USAbilityDatabase* GetAbilityDatabase();

protected:

	UPROPERTY()
	TObjectPtr<USAbilityDatabase> CachedAbilityDatabase;
	
};
