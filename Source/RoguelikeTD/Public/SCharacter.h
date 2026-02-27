// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputAction;
struct FInputActionValue;
struct FInputActionInstance;

UCLASS()
class ROGUELIKETD_API ASCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UAbilitySystemComponent* AbilitySystemComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	class UBasicAttributeSet* BasicAttributeSet;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_Jump;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComp;

	virtual FVector GetPawnViewLocation() const override;

	void Move(const FInputActionValue& InValue);
	void Look(const FInputActionInstance& InValue);

	void PrimaryAttack();
	void Ability1();
	void MobilityAbility();
	void SprintStart();
	void SprintStop();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// GAS Header functions

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

public:

	// The limit for your Roguelike active slots
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	int32 MaxActiveAbilities = 4;

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	bool TryGrantAbility(TSubclassOf<UGameplayAbility> AbilityClass);

	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	FGameplayTag AbilitiesUpdateEventTag;

	UFUNCTION(Exec, Category = "GAS|Debug")
	void DebugGrantAbility(FString AbilityName);

};
