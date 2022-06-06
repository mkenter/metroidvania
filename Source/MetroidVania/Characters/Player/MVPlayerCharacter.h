// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MetroidVania/Characters/MVCharacterBase.h"
#include "MVPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class METROIDVANIA_API AMVPlayerCharacter : public AMVCharacterBase
{
	GENERATED_BODY()

public:

	AMVPlayerCharacter();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;

protected:
	UFUNCTION()
	void Move(float Value);

	UFUNCTION()
	void Duck(float Value);

	bool CanDoubleJump() const;

	class AMVWeapon* SpawnDefaultWeapon() const;

	void EquipWeapon(AMVWeapon* WeaponToEquip); 

private:
	bool bIsDucking;
	bool bJumped;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	AMVWeapon* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMVWeapon> DefaultWeaponClass;

public:

	FORCEINLINE bool GetIsDucking() const { return bIsDucking; }

	FORCEINLINE bool GetJumped() const { return bJumped; }
	FORCEINLINE void SetJumped(const bool NewJumped) { bJumped = NewJumped; }

	FORCEINLINE void GiveDoubleJump() { JumpMaxCount = 2; }
	
};
