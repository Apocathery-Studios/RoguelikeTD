// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"   
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "BasicAttributeSet.h"
#include "UObject/UObjectGlobals.h"
#include <SAssetManager.h>
#include "SAbilityDatabase.h"

// Sets default values
ASCharacter::ASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComp->SetIsReplicated(true);
	AbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	BasicAttributeSet = CreateDefaultSubobject<UBasicAttributeSet>(TEXT("BasicAttributes"));

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInput->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ASCharacter::Move);
	EnhancedInput->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ASCharacter::Look);
	EnhancedInput->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &ASCharacter::Jump);

	/*PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);

	PlayerInputComponent->BindAction("Ability1", IE_Pressed, this, &ASCharacter::Ability1);
	PlayerInputComponent->BindAction("MobilityAbility", IE_Pressed, this, &ASCharacter::MobilityAbility);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintStop);*/

}

void ASCharacter::Move(const FInputActionValue& InValue)
{
	FVector2D InputValue = InValue.Get<FVector2D>();

	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;


	AddMovementInput(ControlRot.Vector(), InputValue.X);

	FVector RightDir = ControlRot.RotateVector(FVector::RightVector);
	AddMovementInput(RightDir, InputValue.Y);
}

void ASCharacter::Look(const FInputActionInstance& InValue)
{
	FVector2D InputValue = InValue.GetValue().Get<FVector2D>();

	AddControllerPitchInput(InputValue.Y);
	AddControllerYawInput(InputValue.X);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

void ASCharacter::PrimaryAttack()
{
	//ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ASCharacter::Ability1() {
	//ActionComp->StartActionByName(this, "Blackhole");
}

void ASCharacter::MobilityAbility()
{
	//ActionComp->StartActionByName(this, "Dash");
}


void ASCharacter::SprintStart()
{
	//ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop()
{
	//ActionComp->StopActionByName(this, "Sprint");
}

// GAS Implementations

UAbilitySystemComponent* ASCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void ASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (AbilitySystemComp) {
		AbilitySystemComp->InitAbilityActorInfo(this, this);
	}
}

void ASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	if (AbilitySystemComp) {
		AbilitySystemComp->InitAbilityActorInfo(this, this);
	}
}

bool ASCharacter::TryGrantAbility(TSubclassOf<UGameplayAbility> AbilityClass)
{
	if (!AbilitySystemComp || !AbilityClass) return false;

	// Count currently held abilities
	int32 ActiveCount = AbilitySystemComp->GetActivatableAbilities().Num();

	if (ActiveCount >= MaxActiveAbilities) return false;

	// 1. Give the Ability
	FGameplayAbilitySpecHandle NewHandle = AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(AbilityClass));

	// 2. Fire the GAS Event instead of the Delegate
	FGameplayEventData Payload;
	Payload.Instigator = this;
	Payload.OptionalObject = AbilityClass; // Pass the class so the UI knows what was added

	AbilitySystemComp->HandleGameplayEvent(AbilitiesUpdateEventTag, &Payload);

	return true;
}

void ASCharacter::TryCastAbilityBySlot(int Slot)
{
	if (!AbilitySystemComp) return;
	TArray<FGameplayAbilitySpec>& Abilities = AbilitySystemComp->GetActivatableAbilities();
	if (Abilities.IsValidIndex(Slot))
	{
		FGameplayAbilitySpec& Spec = Abilities[Slot];
		AbilitySystemComp->TryActivateAbility(Spec.Handle);
	}
}


void ASCharacter::DebugGrantAbility(FString AbilityName)
{
	// 1. Get the Database via our Global Asset Manager
	USAbilityDatabase* DB = USAssetManager::GetAbilityDatabase();
	if (!DB) return;

	// 2. Ask the Database for the class
	UClass* FoundClass = DB->FindAbilityClassByName(AbilityName);

	// 3. Cast to the specific TSubclassOf type TryGrantAbility expects
	TSubclassOf<UGameplayAbility> AbilityClass(FoundClass);

	if (AbilityClass)
	{
		// 4. Call our unified granting function
		if (TryGrantAbility(AbilityClass))
		{
			UE_LOG(LogTemp, Warning, TEXT("DebugGrant: Successfully granted %s via Database."), *AbilityName);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("DebugGrant: TryGrantAbility failed for %s (Likely at Max Skills)."), *AbilityName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DebugGrant: Database could not find ability: %s"), *AbilityName);
	}
}