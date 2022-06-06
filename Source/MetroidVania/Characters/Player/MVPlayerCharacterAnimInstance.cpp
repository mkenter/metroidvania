// Fill out your copyright notice in the Description page of Project Settings.


#include "MVPlayerCharacterAnimInstance.h"

#include "MVPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMVPlayerCharacterAnimInstance::UpdateAnimationProperties(const float DeltaTime)
{
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = GetPlayerCharacter();
	}

	if (PlayerCharacter)
	{
		const UCharacterMovementComponent* CharacterMovementComponent = PlayerCharacter->GetCharacterMovement();

		Velocity = PlayerCharacter->GetVelocity();
		
		FVector SpeedVelocity = Velocity;
		SpeedVelocity.Z = 0;
		Speed = SpeedVelocity.Size();
		bIsInAir = CharacterMovementComponent->IsFalling();
		bIsAccelerating = CharacterMovementComponent->GetCurrentAcceleration().Size() > 0.f;
		bIsDucking = PlayerCharacter->GetIsDucking();
		bJumped = PlayerCharacter->GetJumped();

		if (bJumped)
		{
			PlayerCharacter->SetJumped(false);
		}
		
		const FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(PlayerCharacter->GetVelocity());
		bShouldTurn = MovementRotation.Yaw != LastMovementDirection;
		LastMovementDirection = MovementRotation.Yaw;
		// UE_LOG(LogTemp, Warning, TEXT("bShouldTurn: %s"), bShouldTurn ? TEXT("TRUE") : TEXT("FALSE"));
	}
}

void UMVPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	PlayerCharacter = GetPlayerCharacter();
}

AMVPlayerCharacter* UMVPlayerCharacterAnimInstance::GetPlayerCharacter() const
{
	return Cast<AMVPlayerCharacter>(TryGetPawnOwner());
}
