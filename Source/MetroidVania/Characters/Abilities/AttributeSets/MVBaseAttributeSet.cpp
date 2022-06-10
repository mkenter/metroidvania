// Fill out your copyright notice in the Description page of Project Settings.


#include "MetroidVania/Characters/Abilities/AttributeSets/MVBaseAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "MetroidVania/Characters/MVCharacterBase.h"
#include "Net/UnrealNetwork.h"

UMVBaseAttributeSet::UMVBaseAttributeSet()
{
}

void UMVBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	AMVCharacterBase* TargetCharacter = nullptr;

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		AActor* TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetCharacter = Cast<AMVCharacterBase>(TargetActor);
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		if (LocalDamageDone > 0.f)
		{
			bool bWasAlive = true;

			if (TargetCharacter)
			{
				bWasAlive = TargetCharacter->IsAlive();
			}

			const float NewHealth = GetHealth() - LocalDamageDone;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			// if (TargetCharacter)
			// {
			// 	UE_LOG(LogTemp, Warning, TEXT("Damage done to %s: %f"), *TargetCharacter->GetName(), LocalDamageDone);
			// }

			if (TargetCharacter && bWasAlive && NewHealth == 0.f)
			{
				// UE_LOG(LogTemp, Warning, TEXT("%s died"), *TargetCharacter->GetName());
				TargetCharacter->Die();
			}
		}
	}
}

void UMVBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMVBaseAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMVBaseAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UMVBaseAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMVBaseAttributeSet, Health, OldHealth);
}

void UMVBaseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMVBaseAttributeSet, MaxHealth, OldMaxHealth);
}
