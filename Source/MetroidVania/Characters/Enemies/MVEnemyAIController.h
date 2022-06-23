// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MVEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class METROIDVANIA_API AMVEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	AMVEnemyAIController();

	virtual void SetPawn(APawn* InPawn) override;

	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;
};
