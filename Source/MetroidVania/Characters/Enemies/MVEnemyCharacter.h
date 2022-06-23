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

protected:
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	UPROPERTY(VisibleAnywhere, Category = "AI")
	ACharacter* CurrentTarget;

	UFUNCTION(BlueprintCallable, Category = "Targetting")
	bool SetCurrentTarget(ACharacter* NewTarget);

	UPROPERTY(EditAnywhere)
	class UAIPerceptionComponent* PerceptionComponent;

	UPROPERTY(EditAnywhere)
	class UAISenseConfig_Sight* SenseConfig_Sight;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBlackboardData* BlackboardData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;

public:
	FORCEINLINE UBlackboardData* GetBlackboardData() const { return BlackboardData; }
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
};
