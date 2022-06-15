// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "MVCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class METROIDVANIA_API AMVCameraActor : public ACameraActor
{
	GENERATED_BODY()

public:
	AMVCameraActor();

	virtual void Tick(float DeltaSeconds) override;	
	
};
