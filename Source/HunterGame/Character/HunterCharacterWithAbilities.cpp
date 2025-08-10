// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterCharacterWithAbilities.h"

#include "AbilitySystem/Attributes/HunterCombatSet.h"
#include "AbilitySystem/Attributes/HunterHealthSet.h"
#include "AbilitySystem/HunterAbilitySystemComponent.h"
#include "Async/TaskGraphInterfaces.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterCharacterWithAbilities)

AHunterCharacterWithAbilities::AHunterCharacterWithAbilities(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UHunterAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// These attribute sets will be detected by AbilitySystemComponent::InitializeComponent. Keeping a reference so that the sets don't get garbage collected before that.
	HealthSet = CreateDefaultSubobject<UHunterHealthSet>(TEXT("HealthSet"));
	CombatSet = CreateDefaultSubobject<UHunterCombatSet>(TEXT("CombatSet"));

	// AbilitySystemComponent needs to be updated at a high frequency.
	SetNetUpdateFrequency(100.0f);
}

void AHunterCharacterWithAbilities::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

UAbilitySystemComponent* AHunterCharacterWithAbilities::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

