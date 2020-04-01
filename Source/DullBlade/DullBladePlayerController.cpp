// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "DullBladePlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "DullBladeCharacter.h"
#include "EnemyBase.h"
#include "PickupActor.h"
#include "Chest.h"
#include "InteractionInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"

ADullBladePlayerController::ADullBladePlayerController()
{
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Hand;
}

void ADullBladePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void ADullBladePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ADullBladePlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ADullBladePlayerController::OnSetDestinationReleased);
	InputComponent->BindAction("ToggleRun/Walk", IE_Pressed, this, &ADullBladePlayerController::ToggleWalkRun);
	InputComponent->BindAction("Attack", IE_Pressed, this, &ADullBladePlayerController::Attack);

	InputComponent->BindAction("ScrollUp", IE_Pressed, this, &ADullBladePlayerController::MouseZoomUp);
	InputComponent->BindAction("ScrollDown", IE_Pressed, this, &ADullBladePlayerController::MouseZoomDown);
	InputComponent->BindAction("RotateCamera", IE_Pressed, this, &ADullBladePlayerController::ToggleCameraRotatePressed);
	InputComponent->BindAction("RotateCamera", IE_Released, this, &ADullBladePlayerController::ToggleCameraRotateReleased);

	InputComponent->BindAxis("MoveForward", this, &ADullBladePlayerController::MoveFoward);
	InputComponent->BindAxis("MoveRight", this, &ADullBladePlayerController::MoveRight);
	InputComponent->BindAxis("MouseMiddleAxis", this, &ADullBladePlayerController::RotateCamera);
	
}

void ADullBladePlayerController::MoveFoward(float axisvalue)
{
	APawn* const MyPawn = GetPawn();
	const FRotator Rotation = this->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	MyPawn->AddMovementInput(Direction, axisvalue);
}

void ADullBladePlayerController::MoveRight(float axisvalue)
{
	// find out which way is right
	APawn* const MyPawn = GetPawn();
	const FRotator Rotation = this->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector 
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	// add movement in that direction
	MyPawn->AddMovementInput(Direction, axisvalue);
}

void ADullBladePlayerController::MoveToMouseCursor()
{	
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			if (Hit.GetActor())
			{
				IInteractionInterface* Interaction = Cast<IInteractionInterface>(Hit.GetActor());
				if (Hit.GetActor()->IsA(APickupActor::StaticClass()) || Hit.GetActor()->IsA(AChest::StaticClass()))
				{
					UE_LOG(LogTemp, Warning, TEXT("You hit an Pickup Actor"));
					SetNewMoveDestination(Hit.ImpactPoint);
					Interaction->OnClicked(this);
					return;
				}
				if (Hit.GetActor()->IsA(AEnemyBase::StaticClass()))
				{
					AActor* ActorTarget = Hit.GetActor();
					FVector TargetLocation = ActorTarget->GetActorLocation();
					UE_LOG(LogTemp, Warning, TEXT("You hit an Interface Enemy Class Actor"));
					SetNewMoveDestinationAndAttack(TargetLocation, ActorTarget);
				}
				else
				{
					// We hit something, move there
					SetNewMoveDestination(Hit.ImpactPoint);
				}				
			}		
		}
}

void ADullBladePlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	ADullBladeCharacter* MyPawn = Cast<ADullBladeCharacter>(GetCharacter());
	if (MyPawn)
	{
		
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			MyPawn->GetMesh()->GetAnimInstance()->StopAllMontages(0.5f);
			MyPawn->EndMeleeStrike();
			MyPawn->ResetLastMeleeAttackTime();
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}

	}
}

void ADullBladePlayerController::SetNewMoveDestinationAndAttack(const FVector DestLocation, AActor* Target)
{
	ADullBladeCharacter* MyChar = Cast<ADullBladeCharacter>(GetCharacter());
	
	if (MyChar)
	{
		if (LastTargetedActor != Target)
		{
			MyChar->GetMesh()->GetAnimInstance()->StopAllMontages(.4f);
			MyChar->EndMeleeStrike();
			MyChar->ResetLastMeleeAttackTime();
		}

		float Distance = FVector::Dist(DestLocation, MyChar->GetActorLocation());
		
		if (Distance > 120.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Move Distance is currently: %f "), Distance);
			UAIBlueprintHelperLibrary::SimpleMoveToActor(this, Target);
			return;
		}
		
		if (Distance <= 120.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Melee Distance is currently: %f "), Distance);
			StopMovement();
			MyChar->FaceActor(Target);
			Attack();
			LastTargetedActor = Target;
		}	
	}
}

void ADullBladePlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void ADullBladePlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void ADullBladePlayerController::ToggleWalkRun()
{
	ADullBladeCharacter* MyCharacter = Cast<ADullBladeCharacter>(GetCharacter());
	MyCharacter->SetMovementSpeed();
}

void ADullBladePlayerController::Attack()
{
	ADullBladeCharacter* MyCharacter = Cast<ADullBladeCharacter>(GetCharacter());
	MyCharacter->PlayMeleeStrike();
}

void ADullBladePlayerController::MouseZoomUp()
{
	ADullBladeCharacter* MyPlayerCharacter = Cast<ADullBladeCharacter>(GetCharacter());
	MyPlayerCharacter->MouseZoomIn();
}

void ADullBladePlayerController::MouseZoomDown()
{
	ADullBladeCharacter* MyPlayerCharacter = Cast<ADullBladeCharacter>(GetCharacter());
	MyPlayerCharacter->MouseZoomOut();
}

void ADullBladePlayerController::RotateCamera(float axisvalue)
{
	if (bMiddleButtonToggle)
	{
		ADullBladeCharacter* MyPlayerCharacter = Cast<ADullBladeCharacter>(GetCharacter());
		MyPlayerCharacter->RotateCamera(axisvalue);
	}
}

void ADullBladePlayerController::ToggleCameraRotatePressed()
{
	bMiddleButtonToggle = true;
}

void ADullBladePlayerController::ToggleCameraRotateReleased()
{
	bMiddleButtonToggle = false;
}

