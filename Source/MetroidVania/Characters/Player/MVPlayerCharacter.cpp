// Fill out your copyright notice in the Description page of Project Settings.


#include "MVPlayerCharacter.h"

#include "GameplayTagsManager.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MetroidVania/MetroidVania.h"
#include "MetroidVania/Characters/Abilities/MVBaseAbilitySystemComponent.h"
#include "MetroidVania/Gameplay/MVRoomCameraBounds.h"
#include "MetroidVania/Items/MVWeapon.h"

AMVPlayerCharacter::AMVPlayerCharacter() : bIsDucking(false), bJumped(false),
                                           SpringArmSocketOffsetTarget(FVector(0.f, 0.f, 0.f)),
                                           SpringArmSocketOffsetTargetInterpSpeed(10.f),
                                           CameraCenterDebugSphereColor(FColor::Green)
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f));
	SpringArmComponent->TargetArmLength = 800.0f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	PrimaryActorTick.bCanEverTick = true;
	JumpMaxCount = 2;

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->GravityScale = 2.f;
	MovementComponent->MaxAcceleration = 1'000.f;
	MovementComponent->BrakingFrictionFactor = 0.f;
	MovementComponent->MaxWalkSpeed = 400.f;
	MovementComponent->MaxWalkSpeedCrouched = 200.f;
	MovementComponent->BrakingDecelerationWalking = 200.f;
	MovementComponent->JumpZVelocity = 700.f;
	MovementComponent->BrakingDecelerationFalling = 100.f;
	MovementComponent->AirControl = 0.5f;
	MovementComponent->bImpartBaseVelocityX = false;
	MovementComponent->bImpartBaseVelocityY = false;
	MovementComponent->bImpartBaseVelocityZ = false;
	MovementComponent->bImpartBaseAngularVelocity = false;
	MovementComponent->bConstrainToPlane = true;
	MovementComponent->SetPlaneConstraintNormal(FVector(0.f, 1.f, 0.f));
}

void AMVPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	EquipWeapon(SpawnDefaultWeapon());
	ChangeRooms();
}

void AMVPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateSpringArmSocketOffsetTarget();
	InterpSpringArmSocketOffset(DeltaSeconds);

	if (CameraComponent && CurrentRoomCameraBounds)
	{
		const FVector CameraLocation = CameraComponent->GetComponentLocation();

		if (CameraLocation.X < CurrentRoomCameraBounds->BottomLeftBounds.X + CurrentRoomCameraBounds->WidthOffset ||
			CameraLocation.Z > CurrentRoomCameraBounds->TopRightBounds.Z - CurrentRoomCameraBounds->HeightOffset ||
			CameraLocation.X > CurrentRoomCameraBounds->TopRightBounds.X - CurrentRoomCameraBounds->WidthOffset ||
			CameraLocation.Z < CurrentRoomCameraBounds->BottomLeftBounds.Z + CurrentRoomCameraBounds->HeightOffset)
		{
			CameraCenterDebugSphereColor = FColor::Red;
		}
		else
		{
			CameraCenterDebugSphereColor = FColor::Green;
		}

		DrawDebugSphere(GetWorld(), CameraLocation + FVector(0.f, SpringArmComponent->TargetArmLength * -1.f, 0.f),
		                20.f, 10.f, CameraCenterDebugSphereColor, false);
	}
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
	bShouldLookUp = Value > 0.2f;

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

void AMVPlayerCharacter::UpdateSpringArmSocketOffsetTarget()
{
	SplitSpringArmSocketOffsetTargetInterpSpeed = FVector(SpringArmSocketOffsetTargetInterpSpeed);
	const FVector Velocity = GetVelocity();

	if (Velocity.X > 200.f)
	{
		SpringArmSocketOffsetTarget = FVector(0.f, 400.f, SpringArmSocketOffsetTarget.Z);
	}
	else if (Velocity.X < -200.f)
	{
		SpringArmSocketOffsetTarget = FVector(0.f, -400.f, SpringArmSocketOffsetTarget.Z);
	}
	else if (Velocity.X < 10.f && Velocity.X > -10.f)
	{
		if (bShouldLookUp)
		{
			SpringArmSocketOffsetTarget = FVector(0.f, SpringArmSocketOffsetTarget.Y, 200.f);
		}
		else if (bIsDucking)
		{
			SpringArmSocketOffsetTarget = FVector(0.f, SpringArmSocketOffsetTarget.Y, -200.f);
		}
		else
		{
			SpringArmSocketOffsetTarget = FVector(0.f, SpringArmSocketOffsetTarget.Y, 0.f);
		}
	}
	else
	{
		SpringArmSocketOffsetTarget = FVector(0.f, 0.f, 0.f);
	}

	const float LeftBounds = CurrentRoomCameraBounds->BottomLeftBounds.X + CurrentRoomCameraBounds->WidthOffset;
	const float RightBounds = CurrentRoomCameraBounds->TopRightBounds.X - CurrentRoomCameraBounds->WidthOffset;
	const float BottomBounds = CurrentRoomCameraBounds->BottomLeftBounds.Z + CurrentRoomCameraBounds->HeightOffset;
	const float TopBounds = CurrentRoomCameraBounds->TopRightBounds.Z - CurrentRoomCameraBounds->HeightOffset;

	const FVector CameraLocation = CameraComponent->GetComponentLocation();
	const FVector ActorLocation = GetActorLocation();

	if (ActorLocation.X < LeftBounds || CameraLocation.X < LeftBounds)
	{
		SpringArmSocketOffsetTarget = FVector(
			0.f, FMath::Max(LeftBounds - ActorLocation.X, SpringArmSocketOffsetTarget.Y),
			SpringArmSocketOffsetTarget.Z);
	}

	if (ActorLocation.X > RightBounds || CameraLocation.X > RightBounds)
	{
		SpringArmSocketOffsetTarget = FVector(
			0.f, FMath::Min(RightBounds - ActorLocation.X, SpringArmSocketOffsetTarget.Y),
			SpringArmSocketOffsetTarget.Z);
	}

	if (ActorLocation.Z < BottomBounds || CameraLocation.Z < BottomBounds)
	{
		SpringArmSocketOffsetTarget = FVector(0.f, SpringArmSocketOffsetTarget.Y,
		                                      FMath::Max(BottomBounds - ActorLocation.Z,
		                                                 SpringArmSocketOffsetTarget.Z));
	}

	if (ActorLocation.Z > TopBounds || CameraLocation.Z > TopBounds)
	{
		SpringArmSocketOffsetTarget = FVector(0.f, SpringArmSocketOffsetTarget.Y,
		                                      FMath::Min(TopBounds - ActorLocation.Z, SpringArmSocketOffsetTarget.Z));
	}

	if (CameraLocation.X < LeftBounds || CameraLocation.X > RightBounds)
	{
		SplitSpringArmSocketOffsetTargetInterpSpeed = FVector(SplitSpringArmSocketOffsetTargetInterpSpeed.X, 10.f,
		                                                      SplitSpringArmSocketOffsetTargetInterpSpeed.Z);
	}
	if (CameraLocation.Z < BottomBounds || CameraLocation.Z > TopBounds)
	{
		SplitSpringArmSocketOffsetTargetInterpSpeed = FVector(SplitSpringArmSocketOffsetTargetInterpSpeed.X,
		                                                      SplitSpringArmSocketOffsetTargetInterpSpeed.Y, 10.f);
	}
}

void AMVPlayerCharacter::InterpSpringArmSocketOffset(const float DeltaTime) const
{
	if ((SpringArmComponent->SocketOffset - SpringArmSocketOffsetTarget).Size() > 0.f)
	{
		const float X = FMath::FInterpTo(SpringArmComponent->SocketOffset.X, SpringArmSocketOffsetTarget.X, DeltaTime,
		                                 SplitSpringArmSocketOffsetTargetInterpSpeed.X);
		const float Y = FMath::FInterpTo(SpringArmComponent->SocketOffset.Y, SpringArmSocketOffsetTarget.Y, DeltaTime,
		                                 SplitSpringArmSocketOffsetTargetInterpSpeed.Y);
		const float Z = FMath::FInterpTo(SpringArmComponent->SocketOffset.Z, SpringArmSocketOffsetTarget.Z, DeltaTime,
		                                 SplitSpringArmSocketOffsetTargetInterpSpeed.Z);

		SpringArmComponent->SocketOffset = FVector(X, Y, Z);

		// SpringArmComponent->SocketOffset = FMath::VInterpTo(SpringArmComponent->SocketOffset,
		//                                                     SpringArmSocketOffsetTarget, DeltaTime,
		//                                                     SpringArmSocketOffsetTargetInterpSpeed);
	}
}

void AMVPlayerCharacter::ChangeRooms()
{
	const int32 CurrentRoom = GetCurrentRoom();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMVRoomCameraBounds::StaticClass(), FoundActors);

	for (AActor* CurrentActor : FoundActors)
	{
		const AMVRoomCameraBounds* CurrentRoomBounds = Cast<AMVRoomCameraBounds>(CurrentActor);

		if (CurrentRoomBounds)
		{
			if (CurrentRoomBounds->GetCurrentRoom() == CurrentRoom)
			{
				CurrentRoomCameraBounds = CurrentRoomBounds->GetBounds();
			}
		}
	}
}
