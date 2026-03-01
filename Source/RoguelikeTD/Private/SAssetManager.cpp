// Fill out your copyright notice in the Description page of Project Settings.


#include "SAssetManager.h"
#include "SAbilityDatabase.h"

USAssetManager& USAssetManager::Get()
{
    USAssetManager* This = Cast<USAssetManager>(GEngine->AssetManager);
    if (This) return *This;

    UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini! Must be UMyAssetManager."));
    return *NewObject<USAssetManager>();
}

USAbilityDatabase* USAssetManager::GetAbilityDatabase()
{
    USAssetManager& Manager = Get();

    if (!Manager.CachedAbilityDatabase)
    {
        TArray<FPrimaryAssetId> AssetIds;
        Manager.GetPrimaryAssetIdList(FPrimaryAssetType("AbilityDatabase"), AssetIds);

        if (AssetIds.Num() > 0)
        {
            Manager.CachedAbilityDatabase = Cast<USAbilityDatabase>(Manager.GetPrimaryAssetObject(AssetIds[0]));
        }
    }

    return Manager.CachedAbilityDatabase;
}