// Fill out your copyright notice in the Description page of Project Settings.


#include "MetroidVania/Characters/Enemies/MVEnemyCharacter.h"

#include "MVEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MetroidVania/Characters/Player/MVPlayerCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

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

	SenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SenseConfig_Sight"));
	SenseConfig_Sight->SightRadius = 1000.f;
	SenseConfig_Sight->LoseSightRadius = 2000.f;
	SenseConfig_Sight->PeripheralVisionAngleDegrees = 45.f;
	SenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = true;
	SenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	SenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	PerceptionComponent->ConfigureSense(*SenseConfig_Sight);
	PerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AMVEnemyCharacter::OnPerceptionUpdated);
}

void AMVEnemyCharacter::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	if (CurrentTarget || !UpdatedActors.Num())
	{
		return;
	}

	for (AActor* Actor : UpdatedActors)
	{
		/**
		 * Casting to player character in lieu of teams for now
		 */
		AMVPlayerCharacter* PlayerCharacter = Cast<AMVPlayerCharacter>(Actor);
		
		if (PlayerCharacter)
		{
			/**
			 * Set target class property and BB value
			 */
			const bool bSetTarget = SetCurrentTarget(PlayerCharacter);

			if (bSetTarget)
			{
				break;
			}
		}
	}
}

bool AMVEnemyCharacter::SetCurrentTarget(ACharacter* NewTarget)
{
	AController* MyController = GetController();

	if (MyController)
	{
		AMVEnemyAIController* EnemyAIController = Cast<AMVEnemyAIController>(MyController);

		if (EnemyAIController)
		{
			CurrentTarget = NewTarget;
			EnemyAIController->BlackboardComponent->SetValueAsObject(FName("Target"), NewTarget);
			
			return true;
		}
	}
	
	return false;
}
