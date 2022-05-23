// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MVGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class METROIDVANIA_API AMVGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMVGameModeBase();

protected:
	virtual void BeginPlay() override;
};
