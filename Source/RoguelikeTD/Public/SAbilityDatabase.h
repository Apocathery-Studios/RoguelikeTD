// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SAbilityDataAsset.h"
#include "AbilitySystemComponent.h"
#include "SAbilityDatabase.generated.h"

/**
 * 
 */

class USAbilityDataAsset;
class UGameplayAbility;

UCLASS(BlueprintType)
class ROGUELIKETD_API USAbilityDatabase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

    virtual FPrimaryAssetId GetPrimaryAssetId() const override { 
        return FPrimaryAssetId("AbilityDatabase", GetFName()); 
    }

    UFUNCTION(BlueprintCallable, Category = "Abilities", meta = (DefaultToSelf = "ASC"))
    USAbilityDataAsset* GetInfoForAbilityHandle(UAbilitySystemComponent* ASC, FGameplayAbilitySpecHandle Handle) const;

    UFUNCTION(CallInEditor, Category = "Utility")
    void RefreshMap();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
    TArray<USAbilityDataAsset*> AllAbilities;

    UPROPERTY(Transient)
    TMap<TSubclassOf<UGameplayAbility>, USAbilityDataAsset*> AbilityMap;

    virtual void PostLoad() override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
    UClass* FindAbilityClassByName(FString AbilityName) const;
};
