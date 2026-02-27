// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Abilities/GameplayAbility.h"
#include "SAbilityDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ROGUELIKETD_API USAbilityDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
    // The actual GA class this data belongs to
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    TSubclassOf<UGameplayAbility> AbilityClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSoftObjectPtr<UTexture2D> Icon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    FText AbilityName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (MultiLine = true))
    FText Description;
};
