// Fill out your copyright notice in the Description page of Project Settings.


#include "MetroidVania/Items/MVBaseItem.h"

// Sets default values
AMVBaseItem::AMVBaseItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);
}

// Called when the game starts or when spawned
void AMVBaseItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMVBaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

