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
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	UFUNCTION()
	void Move(float Value);

	UFUNCTION()
	void Duck(float Value);

private:
	bool bIsDucking;

public:

	FORCEINLINE bool GetIsDucking() const { return bIsDucking; }
	
};
