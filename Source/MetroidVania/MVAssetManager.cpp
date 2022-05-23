// Fill out your copyright notice in the Description page of Project Settings.


#include "MVAssetManager.h"
#include "AbilitySystemGlobals.h"

void UMVAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
