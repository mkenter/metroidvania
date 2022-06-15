// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
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
	
	float GetHealth() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	class UMVBaseAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UMVGameplayAbility>> DefaultAbilities;

	int32 GetCurrentRoom() const; 

private:
	UPROPERTY()
	class UMVBaseAttributeSet* BaseAttributeSet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;
	
public:
	virtual void Tick(float DeltaTime) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void Die();

	FORCEINLINE bool IsAlive() const { return GetHealth() > 0.f; }
};
