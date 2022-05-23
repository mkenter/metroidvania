// Fill out your copyright notice in the Description page of Project Settings.


#include "MVPlayerCharacter.h"

#include "MetroidVania/MetroidVania.h"
#include "MetroidVania/Characters/Abilities/MVBaseAbilitySystemComponent.h"

AMVPlayerCharacter::AMVPlayerCharacter()
{
	// JumpMaxCount = 2;
}

void AMVPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMVPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move", this, &AMVPlayerCharacter::Move);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EAbilitySystemInputID",
											  static_cast<int32>(EAbilitySystemInputID::Confirm),
											  static_cast<int32>(EAbilitySystemInputID::Cancel));

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

void AMVPlayerCharacter::Move(float Value)
{
	if (Value == 0.f)
	{
		return;
	}

	const FVector MovementVector = FVector(1.f, 0.f, 0.f);	
	AddMovementInput(MovementVector, Value);

	const float Yaw = Value < 0.f ? 180.f : 0;
	GetController()->SetControlRotation(FRotator(0.f, Yaw, 0.f));
}
