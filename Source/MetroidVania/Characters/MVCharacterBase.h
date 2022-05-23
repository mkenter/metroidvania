// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include <GameplayEffectTypes.h>
#include "MVCharacterBase.generated.h"


UCLASS()
class METROIDVANIA_API AMVCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMVCharacterBase();

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	virtual void InitializeAttributes();
	virtual void GiveAbilities();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	class UMVBaseAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class UMVBaseAttributeSet* BaseAttributeSet;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UMVGameplayAbility>> DefaultAbilities;

public:
	virtual void Tick(float DeltaTime) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	

private:
};
