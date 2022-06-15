// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MVRoomCameraBounds.generated.h"

USTRUCT(BlueprintType)
struct FMVCameraBounds
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	FVector BottomLeftBounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	FVector TopRightBounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float WidthOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float HeightOffset;
};

UCLASS()
class METROIDVANIA_API AMVRoomCameraBounds : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMVRoomCameraBounds();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UBillboardComponent* RootBillboardComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UBillboardComponent* BottomLeftBounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UBillboardComponent* TopRightBounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Location", meta = (AllowPrivateAccess = "true"))
	int32 CurrentRoom;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FMVCameraBounds* GetBounds() const;

	FORCEINLINE int32 GetCurrentRoom() const { return CurrentRoom; }
};
