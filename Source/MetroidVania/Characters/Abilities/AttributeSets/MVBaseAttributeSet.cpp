// Fill out your copyright notice in the Description page of Project Settings.


#include "MetroidVania/Characters/Abilities/AttributeSets/MVBaseAttributeSet.h"
#include "Net/UnrealNetwork.h"

UMVBaseAttributeSet::UMVBaseAttributeSet()
{
}

void UMVBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMVBaseAttributeSet, Health);
}

void UMVBaseAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMVBaseAttributeSet, Health, OldValue);
}
