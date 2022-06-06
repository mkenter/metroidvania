// Fill out your copyright notice in the Description page of Project Settings.


#include "MetroidVania/Items/MVPickup.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "MetroidVania/Characters/Player/MVPlayerCharacter.h"

AMVPickup::AMVPickup()
{
	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetupAttachment(GetRootComponent());
	OverlapSphere->InitSphereRadius(200.f);
	OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	PickupEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PickupEffectComponent"));
	PickupEffectComponent->SetupAttachment(GetRootComponent());
}

void AMVPickup::BeginPlay()
{
	Super::BeginPlay();

	OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &AMVPickup::OnOverlapSphereBeginOverlap);
}

void AMVPickup::Destroyed()
{
	if (PickupEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PickupEffect, GetActorLocation(), GetActorRotation());
	}

	Super::Destroyed();
}

void AMVPickup::OnOverlapSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                            AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp,
                                            int32 OtherBodyIndex,
                                            bool BFromSweep,
                                            const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMVPlayerCharacter* PlayerCharacter = Cast<AMVPlayerCharacter>(OtherActor);

		if (PlayerCharacter)
		{
			ExecutePickup(PlayerCharacter);
			CharacterThatPickedUp = PlayerCharacter;
			Destroy();
		}
	}
}
