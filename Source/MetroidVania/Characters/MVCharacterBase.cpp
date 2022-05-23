// Fill out your copyright notice in the Description page of Project Settings.


#include "MVCharacterBase.h"

#include "Abilities/MVBaseAbilitySystemComponent.h"
#include "Abilities/MVGameplayAbility.h"
#include "Abilities/AttributeSets/MVBaseAttributeSet.h"

AMVCharacterBase::AMVCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UMVBaseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	BaseAttributeSet = CreateDefaultSubobject<UMVBaseAttributeSet>(TEXT("BaseAttributeSet"));
}

void AMVCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMVCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	/**
	 * Server GAS init
	 */
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();
	GiveAbilities();
}

void AMVCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	/**
	 * Client GAS init
	 */
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();

	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EAbilitySystemInputID",
		                                      static_cast<int32>(EAbilitySystemInputID::Confirm),
		                                      static_cast<int32>(EAbilitySystemInputID::Cancel));

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

void AMVCharacterBase::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
			DefaultAttributeEffect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
				*SpecHandle.Data.Get());
		}
	}
}

void AMVCharacterBase::GiveAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UMVGameplayAbility>& StartupAbility : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1,
			                                                         static_cast<int32>(StartupAbility.
				                                                         GetDefaultObject()->AbilityInputID), this));
		}
	}
}

void AMVCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UAbilitySystemComponent* AMVCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
