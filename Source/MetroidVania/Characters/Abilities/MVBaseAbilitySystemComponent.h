// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MVBaseAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class METROIDVANIA_API UMVBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	bool TriggerAbilityByClass(TSubclassOf<UGameplayAbility> InAbilityToActivate, bool bAllowRemoteActivation, FGameplayAbilitySpecHandle& TriggeredAbility);
};
