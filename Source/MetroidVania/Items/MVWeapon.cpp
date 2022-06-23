// Fill out your copyright notice in the Description page of Project Settings.


#include "MetroidVania/Items/MVWeapon.h"

#include "Components/BoxComponent.h"
#include "MetroidVania/Characters/MVCharacterBase.h"
#include "MetroidVania/Characters/Abilities/MVBaseAbilitySystemComponent.h"
#include "MetroidVania/Characters/Abilities/MVGameplayAbility.h"
#include "MetroidVania/Characters/Abilities/AttributeSets/MVBaseAttributeSet.h"
#include "MetroidVania/Characters/Player/MVPlayerCharacter.h"

AMVWeapon::AMVWeapon(): Damage(0.f)
{
	DamageCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageCollisionComponent"));
	DamageCollisionComponent->SetupAttachment(GetRootComponent());
	DamageCollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
	DamageCollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	DamageCollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	AbilitySystemComponent = CreateDefaultSubobject<UMVBaseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	BaseAttributeSet = CreateDefaultSubobject<UMVBaseAttributeSet>(TEXT("BaseAttributeSet"));
}

void AMVWeapon::BeginPlay()
{
	Super::BeginPlay();

	DamageCollisionComponent->OnComponentBeginOverlap.AddDynamic(
		this, &AMVWeapon::DamageCollisionComponentOnOverlapBegin);
	DamageCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMVWeapon::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
			DefaultAttributeEffect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void AMVWeapon::GiveAbilities()
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

void AMVWeapon::DamageCollisionComponentOnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
                                                       AActor* OtherActor,
                                                       UPrimitiveComponent* OtherComp,
                                                       int32 OtherBodyIndex,
                                                       bool BFromSweep,
                                                       const FHitResult& SweepResult)
{
	if (!OtherActor)
	{
		return;
	}

	AMVCharacterBase* CharacterBase = Cast<AMVCharacterBase>(OtherActor);

	if (CharacterBase)
	{
		CollisionDetected(CharacterBase);
	}
}

UAbilitySystemComponent* AMVWeapon::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMVWeapon::OnEquip(AMVPlayerCharacter* NewOwner)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		InitializeAttributes();
		GiveAbilities();
		OwningActor = NewOwner;
	}
}

void AMVWeapon::GiveAbilitiesToOwner()
{
	if (HasAuthority() && OwningActor)
	{
		UAbilitySystemComponent* OwnerAbilitySystemComponent = OwningActor->GetAbilitySystemComponent();

		if (OwnerAbilitySystemComponent)
		{
			for (TSubclassOf<UMVGameplayAbility>& AbilityToGrant : GrantedAbilities)
			{
				OwnerAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToGrant, 1,
				                                                              static_cast<int32>(AbilityToGrant.
					                                                              GetDefaultObject()->AbilityInputID),
				                                                              this));
			}
		}
	}
}

void AMVWeapon::RemoveAbilitiesFromOwner()
{
	if (HasAuthority() && OwningActor)
	{
		UAbilitySystemComponent* OwnerAbilitySystemComponent = OwningActor->GetAbilitySystemComponent();

		if (OwnerAbilitySystemComponent)
		{
			for (TSubclassOf<UMVGameplayAbility>& AbilityToRemove : GrantedAbilities)
			{
			}
		}
	}
}

void AMVWeapon::ActivateCollision()
{
	DamageCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMVWeapon::DeactivateCollision()
{
	DamageCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
