// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MetroidVania/Items/MVBaseItem.h"
#include "AbilitySystemInterface.h"
#include "MVWeapon.generated.h"

/**
 * 
 */
UCLASS()
class METROIDVANIA_API AMVWeapon : public AMVBaseItem, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMVWeapon();

	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* DamageCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class AMVPlayerCharacter* OwningActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	float Damage;

protected:
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UMVGameplayAbility>> GrantedAbilities;

	UFUNCTION()
	void DamageCollisionComponentOnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
									 AActor* OtherActor,
									 UPrimitiveComponent* OtherComp,
									 int32 OtherBodyIndex,
									 bool BFromSweep,
									 const FHitResult& SweepResult);

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayEffect> EquipEffect;

	UFUNCTION()
	void OnEquip(AMVPlayerCharacter* NewOwner);

	UFUNCTION()
	void GiveAbilitiesToOwner();

	UFUNCTION()
	void RemoveAbilitiesFromOwner();

	UFUNCTION(BlueprintCallable)
	void ActivateCollision();
	
	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();

	UFUNCTION(BlueprintImplementableEvent)
	void CollisionDetected(class AMVCharacterBase* CollisionBaseCharacter);

	FORCEINLINE const UBoxComponent* GetDamageCollisionComponent() const { return DamageCollisionComponent; }
};
