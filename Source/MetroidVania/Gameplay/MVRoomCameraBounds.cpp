// Fill out your copyright notice in the Description page of Project Settings.


#include "MetroidVania/Gameplay/MVRoomCameraBounds.h"

#include "Components/BillboardComponent.h"

// Sets default values
AMVRoomCameraBounds::AMVRoomCameraBounds(): CurrentRoom(0)
{
	RootBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("RootBillboardComponent"));
	SetRootComponent(RootBillboardComponent);

	BottomLeftBounds = CreateDefaultSubobject<UBillboardComponent>(TEXT("BottomLeftBounds"));
	BottomLeftBounds->SetupAttachment(RootComponent);
	BottomLeftBounds->SetRelativeTransform(FTransform(FVector(-50.f, 0.f, -50.f)));

	TopRightBounds = CreateDefaultSubobject<UBillboardComponent>(TEXT("TopRightBounds"));
	TopRightBounds->SetupAttachment(RootComponent);
	BottomLeftBounds->SetRelativeTransform(FTransform(FVector(50.f, 0.f, 50.f)));
}

// Called when the game starts or when spawned
void AMVRoomCameraBounds::BeginPlay()
{
	Super::BeginPlay();

	const FMVCameraBounds* Bounds = GetBounds();

	const FVector TopLeft = FVector(Bounds->BottomLeftBounds.X, 110.f, Bounds->TopRightBounds.Z);
	const FVector TopRight = FVector(Bounds->TopRightBounds.X, 110.f, Bounds->TopRightBounds.Z);
	const FVector BottomRight = FVector(Bounds->TopRightBounds.X, 110.f, Bounds->BottomLeftBounds.Z);
	const FVector BottomLeft = FVector(Bounds->BottomLeftBounds.X, 110.f, Bounds->BottomLeftBounds.Z);

	// Left
	DrawDebugLine(GetWorld(), BottomLeft, TopLeft, FColor::Blue, true, -1.f, 0, 5.f);

	// Top
	DrawDebugLine(GetWorld(), TopLeft, TopRight, FColor::Blue, true, -1.f, 0, 5.f);

	// Right
	DrawDebugLine(GetWorld(), TopRight, BottomRight, FColor::Blue, true, -1.f, 0, 5.f);

	// Bottom
	DrawDebugLine(GetWorld(), BottomRight, BottomLeft, FColor::Blue, true, -1.f, 0, 5.f);

	const FVector InsetTopLeft = TopLeft + FVector(Bounds->WidthOffset, 0.f, -1.f * Bounds->HeightOffset);
	const FVector InsetTopRight = TopRight + FVector(-1.f * Bounds->WidthOffset, 0.f, -1 * Bounds->HeightOffset);
	const FVector InsetBottomRight = BottomRight + FVector(-1.f * Bounds->WidthOffset, 0.f, Bounds->HeightOffset);
	const FVector InsetBottomLeft = BottomLeft + FVector(Bounds->WidthOffset, 0.f, Bounds->HeightOffset);

	// Left
	DrawDebugLine(GetWorld(), InsetBottomLeft, InsetTopLeft, FColor::Cyan, true, -1.f, 0, 5.f);

	// Top
	DrawDebugLine(GetWorld(), InsetTopLeft, InsetTopRight, FColor::Cyan, true, -1.f, 0, 5.f);

	// Right
	DrawDebugLine(GetWorld(), InsetTopRight, InsetBottomRight, FColor::Cyan, true, -1.f, 0, 5.f);

	// Bottom
	DrawDebugLine(GetWorld(), InsetBottomRight, InsetBottomLeft, FColor::Cyan, true, -1.f, 0, 5.f);
}

// Called every frame
void AMVRoomCameraBounds::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FMVCameraBounds* AMVRoomCameraBounds::GetBounds() const
{
	FMVCameraBounds* Bounds = new FMVCameraBounds;
	Bounds->BottomLeftBounds = BottomLeftBounds->GetComponentLocation();
	Bounds->TopRightBounds = TopRightBounds->GetComponentLocation();
	Bounds->HeightOffset = 300.f;
	Bounds->WidthOffset = 700.f;

	return Bounds;
}
