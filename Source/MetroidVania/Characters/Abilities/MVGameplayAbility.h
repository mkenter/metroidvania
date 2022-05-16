// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MetroidVania/MetroidVania.h"
#include "MVGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class METROIDVANIA_API UMVGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMVGameplayAbility();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	EAbilitySystemInputID AbilityInputID {EAbilitySystemInputID::None};
};
