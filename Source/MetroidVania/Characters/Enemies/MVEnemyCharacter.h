// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MetroidVania/Characters/MVCharacterBase.h"
#include "MVEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class METROIDVANIA_API AMVEnemyCharacter : public AMVCharacterBase
{
	GENERATED_BODY()

public:
	AMVEnemyCharacter();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBlackboardData* EnemyBlackboard;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* EnemyBehaviorTree;
	
};
