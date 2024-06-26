// Copyright Epic Games, Inc. All Rights Reserved.

#include "TeleportCharacter.h"
#include "TeleportProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TimerManager.h"

//////////////////////////////////////////////////////////////////////////
// ATeleportCharacter

ATeleportCharacter::ATeleportCharacter()
{

	// Character doesnt have a rifle at start
	bHasRifle = false;
	bInterpToRotation = false;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void ATeleportCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

void ATeleportCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Log, TEXT("Player location: %s"), *GetActorLocation().ToString());

	if (bInterpToRotation)
    {
		//UE_LOG(LogTemp, Log, TEXT("interpolating"));
        // Get current rotation
    	FRotator CurrentRotation = Controller->GetControlRotation();

        // Interpolate rotation towards the target rotation smoothly over time
        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, InterpRotationSpeed);

        // Set the new rotation for the controller
        Controller->SetControlRotation(NewRotation);

        // Check if we've reached close enough to the target rotation
        if (FMath::IsNearlyEqual(NewRotation.Yaw, TargetRotation.Yaw, 1.0f))
        {
            // Stop interpolation
            bInterpToRotation = false;
        }
    }

}
//////////////////////////////////////////////////////////////////////////// Input

void ATeleportCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATeleportCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATeleportCharacter::Look);
	}
}


void ATeleportCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ATeleportCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATeleportCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool ATeleportCharacter::GetHasRifle()
{
	return bHasRifle;
}

void ATeleportCharacter::TeleportPlayer(FRotator NewRotation, FVector NewLocation)
{
	UE_LOG(LogTemp, Log, TEXT("start Player location: %s"), *GetActorLocation().ToString());
    if (Controller != nullptr)
    {
        // Disable input
        DisableInput(Cast<APlayerController>(Controller));
		// Set location
        SetActorLocation(NewLocation);
        // Set rotation
		TargetRotation = NewRotation; // 절대각도가 0,0,-90이 목표
        Controller->SetControlRotation(NewRotation + Controller->GetControlRotation()); // 시작각도는 원래 각도 + 0,0,-90
		bInterpToRotation = true;

		SetActorRotation(NewRotation);
        // Calculate the forward direction vector
        MovementDirection = GetActorForwardVector();

        // Set a timer to start moving after 2 seconds
        GetWorld()->GetTimerManager().SetTimer(StartMoveHandle, this, &ATeleportCharacter::StartMoving, 2.0f, false);
    }
	UE_LOG(LogTemp, Log, TEXT("End Player location: %s"), *GetActorLocation().ToString());
}

void ATeleportCharacter::StartMoving()
{
    // Start the move
    GetWorld()->GetTimerManager().SetTimer(MoveForwardHandle, this, &ATeleportCharacter::MoveForward, 0.01f, true);

    // Set a timer to stop movement and re-enable input after 2 seconds
    GetWorld()->GetTimerManager().SetTimer(EnableInputHandle, this, &ATeleportCharacter::EnablePlayerInput, 2.0f, false);
}

void ATeleportCharacter::MoveForward()
{
    // Move incrementally forward by adding movement input
    AddMovementInput(MovementDirection, 1.0f); // Adjust the value to control speed
}

void ATeleportCharacter::EnablePlayerInput()
{
    // Stop moving forward
    GetWorld()->GetTimerManager().ClearTimer(MoveForwardHandle);

    // Enable input
    EnableInput(Cast<APlayerController>(Controller));
}

FVector ATeleportCharacter::ProjectionLocation()
{
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = FVector((-CurrentLocation.X + 1000), 2150, CurrentLocation.Z);
	return NewLocation;
}