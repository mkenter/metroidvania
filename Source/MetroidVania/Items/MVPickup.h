// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MetroidVania/Items/MVBaseItem.h"
#include "MVPickup.generated.h"

/**
 * 
 */
UCLASS()
class METROIDVANIA_API AMVPickup : public AMVBaseItem
{
	GENERATED_BODY()

public:
	AMVPickup();


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* OverlapSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent* PickupEffectComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* PickupEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class AMVPlayerCharacter* CharacterThatPickedUp;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION()
	void OnOverlapSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                                 AActor* OtherActor,
	                                 UPrimitiveComponent* OtherComp,
	                                 int32 OtherBodyIndex,
	                                 bool BFromSweep,
	                                 const FHitResult& SweepResult);

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ExecutePickup(AMVPlayerCharacter* PlayerCharacter);
};
