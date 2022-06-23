// Fill out your copyright notice in the Description page of Project Settings.


#include "MetroidVania/Characters/Enemies/MVEnemyAIController.h"

#include "MVEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

AMVEnemyAIController::AMVEnemyAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AMVEnemyAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	AMVEnemyCharacter* EnemyCharacter = Cast<AMVEnemyCharacter>(InPawn);

	if (EnemyCharacter)
	{
		if (EnemyCharacter->GetBehaviorTree())
		{
			RunBehaviorTree(EnemyCharacter->GetBehaviorTree());
		}

		if (EnemyCharacter->GetBlackboardData())
		{
			UseBlackboard(EnemyCharacter->GetBlackboardData(), BlackboardComponent); 
		}
	}
	
}
