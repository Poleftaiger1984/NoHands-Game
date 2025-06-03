// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Interfaces/InteractionInterface.h"
#include "Items/ASimpleInteractable.h"
#include "Items/Pickup.h"
#include "Items/Playable.h"
#include "Items/GameStates.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

APlayerCharacter::APlayerCharacter()
	: CharacterState(ECharacterState::ECS_Idle)
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->AirControl = 1.f;
	GetCharacterMovement()->AirControlBoostMultiplier = 10000.f;
	GetCharacterMovement()->FallingLateralFriction = 1.f;

	//Control rotation of character with the camera
	GetCharacterMovement()->bOrientRotationToMovement = false;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCamera->bUsePawnControlRotation = true;
	FirstPersonCamera->SetupAttachment(GetRootComponent());
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);

		//Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APlayerCharacter::Crouch);

		//Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::FinishedSprint);

		//Pressing the E Key
		EnhancedInputComponent->BindAction(EKeyPressedAction, ETriggerEvent::Triggered, this, &APlayerCharacter::EKeyPressed);
	}

}

void APlayerCharacter::Jump()
{
	Super::Jump();

	if (bIsMidair && JumpCounter == 1)
	{
		GetCharacterMovement()->Velocity.Z = FMath::Max<FVector::FReal>(GetCharacterMovement()->Velocity.Z, GetCharacterMovement()->JumpZVelocity);
		JumpCounter = 2;
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FString Message = FString::SanitizeFloat(Attributes->GetMoney());

	GEngine->AddOnScreenDebugMessage(1, 100, FColor::Red, Message);

	InteractionTrace(InteractionHit);
	StaminaHandler(DeltaTime);

	if (GetVelocity().Z != 0)
	{
		bIsMidair = true;
	}
	else if (GetVelocity().Z == 0)
	{
		bIsMidair = false;
		JumpCounter = 0;
	}

	if (JumpCounter == 0 && bIsMidair)
	{
		JumpCounter = 1;
	}
}

void APlayerCharacter::StaminaHandler(float DeltaTime)
{
	if ((GetCharacterMovement()->Velocity.X == 0 || GetCharacterMovement()->Velocity.Y == 0 || GetCharacterMovement()->MaxWalkSpeed == WalkSpeed) && Attributes && Attributes->GetStamina() < Attributes->GetMaxStamina() && CharacterState != ECharacterState::ECS_Sprinting)
	{
		Attributes->RegenStamina(DeltaTime);
	}

	if ((GetCharacterMovement()->Velocity.X != 0 || GetCharacterMovement()->Velocity.Y != 0) && Attributes && Attributes->GetStamina() > Attributes->GetSprintCost() && CharacterState == ECharacterState::ECS_Sprinting)
	{
		Attributes->UseStamina(Attributes->GetSprintCost(), DeltaTime);
	}

	if (Attributes && Attributes->GetStamina() < Attributes->GetSprintCost() && CharacterState == ECharacterState::ECS_Sprinting)
	{
		FinishedSprint();
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FPContext, 0);
		}
	}

	Tags.Add(FName("Player"));
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (GetController())
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();
		const FRotator ControllerRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControllerRotation.Yaw, 0.f);

		//Storing which direction the camera/controller is facing
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisValue.X);
	AddControllerPitchInput(LookAxisValue.Y);
}

void APlayerCharacter::Crouch()
{
	if (FirstPersonCamera && CharacterState != ECharacterState::ECS_Crouching)
	{
		FirstPersonCamera->AddWorldOffset(FVector(0.f, 0.f, -CrouchHeight));
		CharacterState = ECharacterState::ECS_Crouching;
		GetCharacterMovement()->MaxWalkSpeed = CrouchWalkSpeed;
	}
	else if (FirstPersonCamera && CharacterState == ECharacterState::ECS_Crouching)
	{
		FirstPersonCamera->AddWorldOffset(FVector(0.f, 0.f, CrouchHeight));
		CharacterState = ECharacterState::ECS_Idle;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void APlayerCharacter::Sprint()
{
	if (!CanSprint()) return;

	CharacterState = ECharacterState::ECS_Sprinting;
	if (Attributes && Attributes->GetStamina() > Attributes->GetSprintCost())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void APlayerCharacter::FinishedSprint()
{
	if (CharacterState == ECharacterState::ECS_Crouching)
	{
		GetCharacterMovement()->MaxWalkSpeed = CrouchWalkSpeed;
	}
	else
	{
		CharacterState = ECharacterState::ECS_Idle;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void APlayerCharacter::EKeyPressed()
{
	IInteractionInterface* InteractionInterface = Cast<IInteractionInterface>(InteractionHit.GetActor()); // We have an item in view
	if (InteractionInterface)
	{
		AASimpleInteractable* SimpleInteractable = Cast<AASimpleInteractable>(InteractionHit.GetActor()); // We have a simple interactable object in view
		if (SimpleInteractable)
		{
			if (SimpleInteractable->ActorHasTag("BoostType"))
			{
				APickup* Pickup = Cast<APickup>(InteractionHit.GetActor()); // We have a pickup in view
				Attributes->AddBoost(Pickup->GetTypeOfBoost(), Pickup->GetAmount());
				Pickup->InteractAction();
			}
			else if (SimpleInteractable->ActorHasTag("Game") && Attributes->GetMoney() > 0)
			{
				APlayable* Playable = Cast<APlayable>(InteractionHit.GetActor()); // We have a game in view
				Attributes->AddBoost(EBoostType::EBT_Money, -Playable->GetGameCost());
				GameState = Playable->GetGameState();

				if (GameState == EGameState::EGS_Idle)
				{
					Playable->SetBet(GameBet);
				}
				if (GameState == EGameState::EGS_Playing)
				{
					Playable->InteractAction();
				}
				
			}
		}
	}
}

void APlayerCharacter::InteractionTrace(FHitResult& SphereHitResult)
{
	const FVector Start = FirstPersonCamera->GetComponentLocation();
	const FVector End = Start + (FirstPersonCamera->GetForwardVector() * InteractRange);

	if (UKismetSystemLibrary::SphereTraceSingle(this, Start, End, CrosshairRadius, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, bShowInteractionDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, SphereHitResult, true))
	{
		SetItemLookedAt(InteractionHit);
	}
}

void APlayerCharacter::SetItemLookedAt(FHitResult& LookTraceResult)
{
	TObjectPtr<AActor> HitActor = LookTraceResult.GetActor();
	if (HitActor != LastSeenActor)
	{
		//The looked at object has changed
		LastSeenActor = HitActor;
		ItemInView = nullptr;

		if (HitActor && HitActor->IsA<AASimpleInteractable>())
		{
			ItemInView = Cast<AASimpleInteractable>(HitActor); //Current Item Looked At
			UE_LOG(LogTemp, Warning, TEXT("%s"), *ItemInView.GetName());
		}
	}
}

bool APlayerCharacter::CanSprint()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetSprintCost() && CharacterState != ECharacterState::ECS_Sprinting && CharacterState != ECharacterState::ECS_Crouching;
}