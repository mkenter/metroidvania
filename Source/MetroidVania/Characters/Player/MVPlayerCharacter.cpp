// Fill out your copyright notice in the Description page of Project Settings.


#include "MVPlayerCharacter.h"

#include "GameplayTagsManager.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MetroidVania/MetroidVania.h"
#include "MetroidVania/Characters/Abilities/MVBaseAbilitySystemComponent.h"
#include "MetroidVania/Items/MVWeapon.h"

AMVPlayerCharacter::AMVPlayerCharacter() : bIsDucking(false), bJumped(false)
{
	PrimaryActorTick.bCanEverTick = true;
	JumpMaxCount = 2;
}

void AMVPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	EquipWeapon(SpawnDefaultWeapon());
}

void AMVPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMVPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move", this, &AMVPlayerCharacter::Move);
	PlayerInputComponent->BindAxis("Duck", this, &AMVPlayerCharacter::Duck);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EAbilitySystemInputID",
		                                       static_cast<int32>(EAbilitySystemInputID::Confirm),
		                                       static_cast<int32>(EAbilitySystemInputID::Cancel));

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

void AMVPlayerCharacter::Jump()
{
	if (JumpCurrentCount > 0 && !CanDoubleJump())
	{
		return;
	}
	
	Super::Jump();
	
	if (JumpCurrentCount < JumpMaxCount)
	{
		bJumped = true;
	}
}

void AMVPlayerCharacter::Move(float Value)
{
	if (Value == 0.f)
	{
		return;
	}

	const float Yaw = Value < 0.f ? 180.f : 0;
	GetController()->SetControlRotation(FRotator(0.f, Yaw, 0.f));

	if (!bIsDucking)
	{
		const FVector MovementVector = FVector(1.f, 0.f, 0.f);
		AddMovementInput(MovementVector, Value);
	}
}

void AMVPlayerCharacter::Duck(float Value)
{
	if (Value > -0.2f || GetCharacterMovement()->IsFalling())
	{
		bIsDucking = false;
		return;
	}

	bIsDucking = true;
}

bool AMVPlayerCharacter::CanDoubleJump() const
{
	FGameplayTagContainer TagContainer;
	const FGameplayTag DoubleJumpTag = UGameplayTagsManager::Get().RequestGameplayTag(
		"Player.Abilities.Passive.Gained.DoubleJump");

	AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);

	return TagContainer.HasTag(DoubleJumpTag);
}

AMVWeapon* AMVPlayerCharacter::SpawnDefaultWeapon() const
{
	if (DefaultWeaponClass)
	{
		return GetWorld()->SpawnActor<AMVWeapon>(DefaultWeaponClass);
	}

	return nullptr;
}

void AMVPlayerCharacter::EquipWeapon(AMVWeapon* WeaponToEquip)
{
	if (WeaponToEquip)
	{
		WeaponToEquip->OnEquip(this);

		const USkeletalMeshSocket* WeaponSocket = GetMesh()->GetSocketByName(FName("WeaponSocket"));

		if (WeaponSocket)
		{
			WeaponSocket->AttachActor(WeaponToEquip, GetMesh());
		}

		EquippedWeapon = WeaponToEquip;
	}
}
