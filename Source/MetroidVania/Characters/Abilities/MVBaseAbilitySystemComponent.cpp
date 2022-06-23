// Fill out your copyright notice in the Description page of Project Settings.


#include "MetroidVania/Characters/Abilities/MVBaseAbilitySystemComponent.h"

bool UMVBaseAbilitySystemComponent::TriggerAbilityByClass(TSubclassOf<UGameplayAbility> InAbilityToActivate, bool bAllowRemoteActivation, FGameplayAbilitySpecHandle& TriggeredAbility)
{
	bool bSuccess = false;

	const UGameplayAbility* const InAbilityCDO = InAbilityToActivate.GetDefaultObject();

	for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability == InAbilityCDO)
		{
			bSuccess |= TryActivateAbility(Spec.Handle, bAllowRemoteActivation);
			TriggeredAbility = Spec.Handle;
			break;
		}
	}

	return bSuccess;
}