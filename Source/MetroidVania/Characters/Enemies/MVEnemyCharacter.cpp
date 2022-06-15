// Fill out your copyright notice in the Description page of Project Settings.


#include "MetroidVania/Characters/Enemies/MVEnemyCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

AMVEnemyCharacter::AMVEnemyCharacter()
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->GravityScale = 2.f;
	MovementComponent->MaxAcceleration = 1'000.f;
	MovementComponent->BrakingFrictionFactor = 0.f;
	MovementComponent->MaxWalkSpeed = 400.f;
	MovementComponent->MaxWalkSpeedCrouched = 200.f;
	MovementComponent->BrakingDecelerationWalking = 200.f;
	MovementComponent->JumpZVelocity = 700.f;
	MovementComponent->BrakingDecelerationFalling = 100.f;
	MovementComponent->AirControl = 0.5f;
	MovementComponent->bImpartBaseVelocityX = false;
	MovementComponent->bImpartBaseVelocityY = false;
	MovementComponent->bImpartBaseVelocityZ = false;
	MovementComponent->bImpartBaseAngularVelocity = false;
	MovementComponent->bConstrainToPlane = true;
	MovementComponent->SetPlaneConstraintNormal(FVector(0.f, 1.f, 0.f));
}
