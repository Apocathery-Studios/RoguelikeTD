#include "SAbilityDatabase.h"
#include "SAbilityDataAsset.h"
#include "Abilities/GameplayAbility.h"

USAbilityDataAsset* USAbilityDatabase::GetInfoForAbilityHandle(UAbilitySystemComponent* ASC, FGameplayAbilitySpecHandle Handle) const
{
    if (AbilityMap.IsEmpty() && !AllAbilities.IsEmpty())
    {
        const_cast<USAbilityDatabase*>(this)->RefreshMap();
    }

    if (!ASC || !Handle.IsValid())
    {
        return nullptr;
    }

    FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(Handle);

    if (Spec && Spec->Ability)
    {
        TSubclassOf<UGameplayAbility> AbilityClass = Spec->Ability->GetClass();

        if (USAbilityDataAsset* const* FoundData = AbilityMap.Find(AbilityClass))
        {
            return *FoundData;
        }
    }

    return nullptr;
}

void USAbilityDatabase::RefreshMap()
{
    AbilityMap.Empty();

    for (USAbilityDataAsset* Data : AllAbilities)
    {
        if (Data && Data->AbilityClass)
        {
            AbilityMap.Add(Data->AbilityClass, Data);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Ability Database '%s' refreshed. Map size: %d"), *GetName(), AbilityMap.Num());
}

void USAbilityDatabase::PostLoad()
{
    Super::PostLoad();
    RefreshMap();
}

#if WITH_EDITOR
void USAbilityDatabase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    const FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

    if (PropertyName == GET_MEMBER_NAME_CHECKED(USAbilityDatabase, AllAbilities))
    {
        RefreshMap();
    }
}
#endif

UClass* USAbilityDatabase::FindAbilityClassByName(FString AbilityName) const
{
    for (const auto& Entry : AbilityMap)
    {
        UClass* AbilityClass = Entry.Key;
        if (AbilityClass)
        {
            if (AbilityClass->GetName().Contains(AbilityName))
            {
                return AbilityClass;
            }
        }
    }
    return nullptr;
}