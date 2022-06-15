// Fill out your copyright notice in the Description page of Project Settings.


#include "MVGameModeBase.h"

#include "Kismet/GameplayStatics.h"

AMVGameModeBase::AMVGameModeBase(): CurrentRoom(0)
{
}

void AMVGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}
